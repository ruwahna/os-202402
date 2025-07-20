# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi

**Semester**: Genap / Tahun Ajaran 2024–2025

**Nama**: Indah Ruwahna Anugraheni

**NIM**: 240202866

**Modul yang Dikerjakan**:
(Modul 3 – Copy-on-Write Fork dan Shared Memory ala System V)

## 📌 Deskripsi Singkat Tugas
Modul ini mengimplementasikan dua fitur memori di xv6:
1. Copy-on-Write (CoW): Proses child dan parent berbagi halaman setelah fork(),      dan hanya menyalin halaman saat terjadi penulisan.
2. Shared Memory: Dua proses dapat berbagi data melalui syscall shmget() dan         shmrelease() menggunakan ID yang sama.


---

## 🛠️ Rincian Implementasi
**Copy-on-Write:**
  * Mengubah fungsi fork() agar tidak langsung menyalin seluruh halaman memori.
  * Menandai halaman sebagai read-only dan berbagi halaman antar proses.
  * Menangani page fault saat child/parent menulis halaman — membuat salinan baru      (copy-on-write).
  * Menambahkan fungsi baru: cowuvm() dan modifikasi pagefault() di trap.c.

**Shared Memory ala System V:**
  * Menambahkan dua syscall: shmget() dan shmrelease()
  * Mengelola shared memory dengan ID unik (misalnya dari 0–7).
  * Halaman dibagikan pada alamat USERTOP - PGSIZE * (id+1)
  * Menyimpan status penggunaan shared memory dan reference count.

**File yang Dimodifikasi:**
  * vm.c, trap.c, proc.c, syscall.c, sysproc.c, usys.S, defs.h, memlayout.h,          user.h, syscall.h, dll.

---

## ✅ Program Uji

**1. cowtest**
* Menguji CoW saat fork() dipanggil.
* Parent dan child berbagi halaman, tapi saat salah satu menulis, salinan           dibuat.
**Output**
    Child sees: Y
    Parent sees: X
     
**2. shmtest**

     * Menguji alokasi shared memory antar dua proses.
     * Menulis di satu proses, membaca di proses lain.
   **Output**
     Child reads: A
     Parent reads: B

     
---

## 📷 Screenshot
**cowtest**
<img width="759" height="316" alt="cowtest" src="https://github.com/user-attachments/assets/84872c26-221c-4cc3-ab57-a57a55d8c73d" />

**shmtest**
<img width="795" height="307" alt="shmtest" src="https://github.com/user-attachments/assets/c99e8788-260d-485f-99c5-76f3cf72c34b" />



---

## ⚠️ Kendala yang Dihadapi

1. Panic saat page fault karena lupa mengatur bit permission read-only dengan         benar.
2. Halaman tidak dicopy saat terjadi write (karena lupa cek COW flag).
3. Kesalahan offset pada USERTOP menyebabkan shared memory overwrite stack.
4. Syscall shmget() tidak berfungsi jika lupa mendaftarkan syscall atau page tidak    di-map.


---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


