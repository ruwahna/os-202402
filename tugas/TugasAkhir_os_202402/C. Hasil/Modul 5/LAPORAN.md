# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi

**Semester**: Genap / Tahun Ajaran 2024–2025

**Nama**: Indah Ruwahna Anugraheni

**NIM**: 240202866

**Modul yang Dikerjakan**:

(Modul 5 – Audit dan Keamanan Sistem)

---

## 📌 Deskripsi Singkat Tugas

Modul ini menambahkan fitur audit log ke kernel xv6 untuk mencatat informasi setiap pemanggilan syscall. Audit log ini hanya dapat diakses oleh proses dengan PID 1 melalui syscall get_audit_log() dan ditampilkan oleh program user-space bernama audit.

---

## 🛠️ Rincian Implementasi

1. Membuat struktur struct audit_entry untuk menyimpan pid, syscall_num, dan tick
2. Menambahkan buffer log dan variabel index di kernel
3. Menambahkan fungsi log_syscall() dan memanggilnya di setiap syscall
4. Implementasi syscall baru get_audit_log() di sysproc.c dan syscall.c
5. Meregistrasikan syscall di syscall.h, usys.S, dan user.h
6. Membuat program user-space audit.c untuk menampilkan isi log
7. Menambahkan audit ke Makefile dalam daftar UPROGS
---

## ✅ Uji Fungsionalitas

1. audit: menampilkan isi audit log jika dijalankan oleh PID 1
2. Syscall apapun yang dipanggil akan tercatat secara otomatis
3. Audit log tidak bisa diakses oleh proses selain PID 1

---

## 📷 Hasil Uji

**Output audit oleh PID ≠ 1**
```
$ audit
Access denied or error.
```
**Output audit oleh PID = 1**
```
=== Audit Log ===
[0] PID=1 SYSCALL=5 TICK=2
[1] PID=1 SYSCALL=6 TICK=2
[2] PID=1 SYSCALL=10 TICK=3
```
---
## 📷 Screenshot

<img width="923" height="444" alt="audit_modul5" src="https://github.com/user-attachments/assets/7495bc4d-a0e1-430f-9c79-6621e057144f" />

---

## ⚠️ Kendala yang Dihadapi

1. Audit log hanya bisa diakses oleh PID 1, sehingga perlu mengedit init.c untuk      pengujian
2. Kadang log kosong jika belum ada syscall dipanggil
3. Log memiliki batasan jumlah (misal 128 entri), perlu dijaga agar tidak overflow
4. Salah pointer atau argumen di syscall bisa menyebabkan log tidak terbaca

---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


