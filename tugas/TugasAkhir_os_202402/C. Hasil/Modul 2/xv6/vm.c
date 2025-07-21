#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"
#include "string.h"

extern char data[];  // defined by kernel.ld
pde_t *kpgdir;       // kernel page table

// Tambahan untuk Copy-On-Write
#define NPAGE (PHYSTOP >> 12)
int ref_count[NPAGE];

void incref(char *pa) {
  ref_count[V2P(pa) >> 12]++;
}

void decref(char *pa) {
  int idx = V2P(pa) >> 12;
  if (--ref_count[idx] == 0)
    kfree(pa);
}

// Set up kernel part of a page table.
pde_t* setupkvm(void) {
  pde_t *pgdir;
  struct kmap {
    void *virt;
    uint phys_start;
    uint phys_end;
    int perm;
  } kmap[] = {
    { (void*)KERNBASE, 0,             EXTMEM,    PTE_W}, // I/O space
    { (void*)KERNLINK, V2P(KERNLINK), V2P(data), 0},     // kern text+rodata
    { (void*)data,     V2P(data),     PHYSTOP,   PTE_W}, // kern data+memory
  };

  if((pgdir = (pde_t*)kalloc()) == 0)
    return 0;
  memset(pgdir, 0, PGSIZE);

  struct kmap *k;
  for(k = kmap; k < &kmap[NELEM(kmap)]; k++){
    if(mappages(pgdir, k->virt, k->phys_end - k->phys_start,
                (uint)k->phys_start, k->perm) < 0){
      freevm(pgdir);
      return 0;
    }
  }
  return pgdir;
}

void switchkvm(void) {
  lcr3(V2P(kpgdir));   // switch to the kernel page table
}

void switchuvm(struct proc *p) {
  if(p == 0)
    panic("switchuvm: no process");
  if(p->kstack == 0)
    panic("switchuvm: no kstack");
  if(p->pgdir == 0)
    panic("switchuvm: no pgdir");

  pushcli();
  lcr3(V2P(p->pgdir));
  popcli();
}

void kvmalloc(void) {
  kpgdir = setupkvm();
  if(kpgdir == 0)
    panic("kvmalloc");
  switchkvm();
}

pte_t* walkpgdir(pde_t *pgdir, const void *va, int alloc) {
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
  } else {
    if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
      return 0;
    memset(pgtab, 0, PGSIZE);
    *pde = V2P(pgtab) | PTE_P | PTE_W | PTE_U;
  }
  return &pgtab[PTX(va)];
}

int mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm) {
  char *a, *last;
  pte_t *pte;

  a = (char*)PGROUNDDOWN((uint)va);
  last = (char*)PGROUNDDOWN(((uint)va) + size - 1);
  for(;;){
    if((pte = walkpgdir(pgdir, a, 1)) == 0)
      return -1;
    if(*pte & PTE_P)
      panic("mappages: remap");
    *pte = pa | perm | PTE_P;
    if(a == last)
      break;
    a += PGSIZE;
    pa += PGSIZE;
  }
  return 0;
}

void freevm(pde_t *pgdir) {
  if(pgdir == 0)
    panic("freevm: no pgdir");

  for(uint i = 0; i < NPDENTRIES; i++){
    if(!(pgdir[i] & PTE_P))
      continue;
    pte_t *pgtab = (pte_t*)P2V(PTE_ADDR(pgdir[i]));
    for(uint j = 0; j < NPTENTRIES; j++){
      if(pgtab[j] & PTE_P){
        char *v = P2V(PTE_ADDR(pgtab[j]));
        decref(v);
      }
    }
    kfree((char*)pgtab);
  }
  kfree((char*)pgdir);
}

int allocuvm(pde_t *pgdir, uint oldsz, uint newsz) {
  char *mem;
  uint a;

  if(newsz >= KERNBASE)
    return 0;
  if(newsz < oldsz)
    return oldsz;

  a = PGROUNDUP(oldsz);
  for(; a < newsz; a += PGSIZE){
    mem = kalloc();
    if(mem == 0){
      cprintf("allocuvm out of memory\n");
      deallocuvm(pgdir, newsz, oldsz);
      return 0;
    }
    memset(mem, 0, PGSIZE);
    if(mappages(pgdir, (void*)a, PGSIZE, V2P(mem), PTE_W|PTE_U) < 0){
      cprintf("allocuvm: mappages failed\n");
      deallocuvm(pgdir, newsz, oldsz);
      kfree(mem);
      return 0;
    }
  }
  return newsz;
}

int deallocuvm(pde_t *pgdir, uint oldsz, uint newsz) {
  pte_t *pte;
  uint a;
  char *v;

  if(newsz >= oldsz)
    return oldsz;

  a = PGROUNDUP(newsz);
  for(; a < oldsz; a += PGSIZE){
    pte = walkpgdir(pgdir, (char*)a, 0);
    if(!pte || !(*pte & PTE_P))
      continue;
    v = P2V(PTE_ADDR(*pte));
    decref(v);
    *pte = 0;
  }
  return newsz;
}

void clearpteu(pde_t *pgdir, char *uva) {
  pte_t *pte;

  pte = walkpgdir(pgdir, uva, 0);
  if(pte == 0)
    panic("clearpteu");
  *pte &= ~PTE_U;
}

int copyout(pde_t *pgdir, uint va, void *p, uint len) {
  char *buf = (char*)p;
  uint n, pa;
  pte_t *pte;

  while(len > 0){
    pte = walkpgdir(pgdir, (void*)va, 0);
    if(!pte || !(*pte & PTE_P))
      return -1;
    pa = PTE_ADDR(*pte);
    uint n = PGSIZE - (va % PGSIZE);
    if(n > len) n = len;
    memmove((void*)(pa + (va % PGSIZE)), buf, n);
    len -= n;
    buf += n;
    va += n;
  }
  return 0;
}

int loaduvm(pde_t *pgdir, char *addr, struct inode *ip, uint offset, uint sz) {
  uint i, n;
  pte_t *pte;
  uint pa;

  if((uint)addr % PGSIZE != 0)
    panic("loaduvm: addr must be page aligned");

  for(i = 0; i < sz; i += PGSIZE){
    pte = walkpgdir(pgdir, addr + i, 0);
    if(!pte)
      panic("loaduvm: address should exist");
    pa = PTE_ADDR(*pte);
    if(readi(ip, P2V(pa), offset + i, PGSIZE) != PGSIZE)
      return -1;
  }
  return 0;
}

int inituvm(pde_t *pgdir, char *init, uint sz) {
  char *mem;

  if(sz >= PGSIZE)
    panic("inituvm: more than a page");
  mem = kalloc();
  memset(mem, 0, PGSIZE);
  mappages(pgdir, 0, PGSIZE, V2P(mem), PTE_W|PTE_U);
  memmove(mem, init, sz);
  return 0;
}

// Tambahan: Copy-On-Write version of copyuvm
pde_t* cowuvm(pde_t *pgdir, uint sz) {
  pde_t *d = setupkvm();
  if(d == 0)
    return 0;

  for(uint i = 0; i < sz; i += PGSIZE){
    pte_t *pte = walkpgdir(pgdir, (void*)i, 0);
    if(!pte || !(*pte & PTE_P))
      continue;

    uint pa = PTE_ADDR(*pte);
    char *v = P2V(pa);
    uint flags = PTE_FLAGS(*pte);

    flags &= ~PTE_W;
    flags |= PTE_COW;

    incref(v);

    if(mappages(d, (void*)i, PGSIZE, pa, flags) < 0){
      freevm(d);
      return 0;
    }

    *pte = (*pte & ~PTE_W) | PTE_COW;
  }
  return d;
}
