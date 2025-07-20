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

Tuliskan program uji apa saja yang Anda gunakan, misalnya:

* `ptest`: untuk menguji `getpinfo()`
* `rtest`: untuk menguji `getReadCount()`
* `cowtest`: untuk menguji fork dengan Copy-on-Write
* `shmtest`: untuk menguji `shmget()` dan `shmrelease()`
* `chmodtest`: untuk memastikan file `read-only` tidak bisa ditulis
* `audit`: untuk melihat isi log system call (jika dijalankan oleh PID 1)

---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `cowtest`:

```
Child sees: Y
Parent sees: X
```

### 📍 Contoh Output `shmtest`:

```
Child reads: A
Parent reads: B
```

### 📍 Contoh Output `chmodtest`:

```
Write blocked as expected
```

Jika ada screenshot:

```
![hasil cowtest](./screenshots/cowtest_output.png)
```

---

## ⚠️ Kendala yang Dihadapi

Tuliskan kendala (jika ada), misalnya:

* Salah implementasi `page fault` menyebabkan panic
* Salah memetakan alamat shared memory ke USERTOP
* Proses biasa bisa akses audit log (belum ada validasi PID)

---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


