# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi

**Semester**: Genap / Tahun Ajaran 2024–2025

**Nama**: Indah Ruwahna Anugraheni

**NIM**: 240202866

**Modul yang Dikerjakan**:
`( Modul 1 – System Call dan Instrumentasi Kernel)`

---

## 📌 Deskripsi Singkat Tugas

**Modul 1 – System Call dan Instrumentasi Kernel**:

Modul ini berfokus pada penambahan dua buah system call baru dalam kernel xv6-public:

1. getpinfo(struct pinfo *ptable)
→ Untuk mendapatkan informasi proses yang aktif, yaitu PID, ukuran memori, dan nama proses.

2. getReadCount()
→ Untuk menghitung jumlah total pemanggilan read() sejak sistem di-boot.

Modifikasi ini bertujuan untuk mempraktikkan mekanisme system call di dalam kernel dan menambahkan instrumentasi dasar untuk pelacakan aktivitas sistem.
  
---

## 🛠️ Rincian Implementasi

1. Menambahkan struktur struct pinfo di proc.h untuk menampung informasi proses aktif.

2. Menambahkan variabel global readcount di sysproc.c untuk mencatat jumlah pemanggilan read().

3. Mendeklarasikan nomor syscall baru di syscall.h (SYS_getpinfo dan SYS_getreadcount).

4. Mendaftarkan syscall ke dalam tabel syscalls[] di syscall.c.

5. Menambahkan deklarasi syscall di user.h dan implementasi stub di usys.S.

6. Implementasi fungsi sys_getpinfo() dan sys_getreadcount() di sysproc.c.

7. Menambahkan instruksi readcount++ di awal fungsi sys_read() di sysfile.c.

8. Menyusun dua program user-level untuk menguji kedua syscall: ptest.c dan rtest.c.

9. Menambahkan ptest dan rtest ke bagian UPROGS pada Makefile.


---

## ✅ Uji Fungsionalitas

Program yang digunakan untuk pengujian:

1. ptest → Menguji apakah getpinfo() berhasil menampilkan informasi proses yang sedang aktif.

2. rtest → Menguji apakah getReadCount() menghitung jumlah read() secara akurat.
   
---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `ptest`:

```
== Info Proses Aktif ==
PID     MEM     NAME
1       4096    init
2       2048    sh
3       2048    ptest
```

### 📍 Contoh Output `rtest`:

```
Read Count Sebelum: 4
hello
Read Count Setelah: 5

```

<img width="566" height="397" alt="ptest_rtest_hello" src="https://github.com/user-attachments/assets/b87618b7-cde2-4c8d-a904-e98cc62978ee" />

---

## ⚠️ Kendala yang Dihadapi

1. Kesalahan Format atau Tipe Data
Kesalahan seperti menggunakan tanda . alih-alih -> saat mengakses struct pointer, atau mendeklarasikan variabel dengan tipe yang salah sering menyebabkan error saat kompilasi atau hasil yang tidak sesuai.
2. Syscall tidak dikenali
Terjadi karena lupa menambahkan entri di syscall.h, syscall.c, dan usys.S.
3. Kernel panic saat akses pointer
Salah ukuran pada argptr() menyebabkan crash; diperbaiki dengan memastikan ukuran struct benar.
4. Nilai struct tidak terbaca
Pointer dari user-space belum valid atau tidak dialokasikan dengan benar.
5. Program uji tidak muncul di xv6
Lupa menambahkan ke UPROGS di Makefile, sehingga tidak terinstall.

---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


