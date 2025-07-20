# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi

**Semester**: Genap / Tahun Ajaran 2024–2025

**Nama**: Indah Ruwahna Anugraheni

**NIM**: 240202866

**Modul yang Dikerjakan**:

(Modul 2 – Penjadwalan CPU Non-Preemptive Berbasis Prioritas)

---

## 📌 Deskripsi Singkat Tugas

* **Modul 2 Penjadwalan CPU Non-Preemptive Berbasis Prioritas**:
Mengubah algoritma penjadwalan proses di kernel xv6-public dari Round Robin menjadi Non-Preemptive Priority Scheduling. Setiap proses memiliki nilai prioritas (semakin kecil angkanya, semakin tinggi prioritasnya), dan scheduler hanya akan menjalankan proses RUNNABLE dengan prioritas tertinggi tanpa melakukan preemption.
---

## 🛠️ Rincian Implementasi

1. Menambahkan field priority pada struct proc di proc.h
2. Menginisialisasi priority di allocproc() dalam proc.c
3. Menambahkan syscall set_priority(int) untuk mengatur nilai prioritas proses
4. Melakukan perubahan pada:
   *syscall.h, syscall.c, user.h, dan usys.S untuk pendaftaran syscall
   *sysproc.c untuk implementasi syscall
5. Memodifikasi fungsi scheduler() di proc.c untuk memilih proses RUNNABLE dengan prioritas tertinggi
6. Membuat program pengujian ptest.c
7. Menambahkan ptest ke dalam Makefile pada bagian UPROGS
---

## ✅ Uji Fungsionalitas

1. ptest modul2: menguji penjadwalan berdasarkan prioritas
   
   * Child 2 diberi prioritas tinggi (10)
   
   * Child 1 diberi prioritas rendah (90)
   
   * Diharapkan output: Child 2 selesai muncul sebelum Child 1 selesai

---

## 📷 Hasil Uji

### 📍ptest:

```
Child 2 selesai
Child 1 selesai
Parent selesai
```
### 📷 Screenshot:


<img width="725" height="337" alt="ptest_modul2" src="https://github.com/user-attachments/assets/189d6f92-f39a-4e2a-9e2c-e258778f3178" />


---

## ⚠️ Kendala yang Dihadapi

1. Prioritas Tidak Terbaca oleh Scheduler
   Field priority sudah ditambahkan ke struct proc, tapi lupa dipertimbangkan di fungsi scheduler(), jadi tetap pakai urutan default.
2. Semua Proses Jalan Berurutan Seperti Round Robin
   Karena tidak ada sorting atau seleksi berdasarkan prioritas di loop scheduler(), hasilnya seperti belum berubah.
3. Set Priority Tidak Efektif
   Syscall set_priority() tidak mengubah nilai prioritas dengan benar (misalnya lupa p->priority = n), atau tidak diset pada proses anak.
4. Kesalahan Nomor Syscall
   Salah menambahkan nomor syscall baru di syscall.h, atau lupa mendaftarkan syscall baru di syscall.c dan usys.S.
5. Kernel Panic atau Kompilasi Gagal
   Salah pointer atau struct saat memodifikasi proc.c dan proc.h, misalnya akses ke proses NULL atau belum terinisialisasi.


---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


