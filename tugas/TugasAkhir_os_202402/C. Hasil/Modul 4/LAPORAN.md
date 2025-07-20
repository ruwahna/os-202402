# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi

**Semester**: Genap / Tahun Ajaran 2024–2025

**Nama**: Indah Ruwahna Anugraheni

**NIM**: 240202866

**Modul yang Dikerjakan**:

(Modul 4 – Subsistem Kernel Alternatif (chmod() dan /dev/random))

---

## 📌 Deskripsi Singkat Tugas

Modul ini berfokus pada penambahan dua fitur baru ke kernel xv6, yaitu:

1. System Call chmod(path, mode) – untuk mengatur izin akses file menjadi read-      only (mode = 1) atau read-write (mode = 0).
2. Device /dev/random – sebuah device file yang menghasilkan angka acak setiap       kali dibaca.
---

## 🛠️ Rincian Implementasi

**Fitur chmod(path, mode)**
1. Menambahkan syscall chmod() pada sysfile.c.
2. Menambah field mode di struct inode pada fs.h.
3. Menyesuaikan fungsi filewrite() agar memeriksa f->ip->mode == 1 sebelum           menulis.
4. Registrasi syscall di syscall.c, syscall.h, user.h, dan usys.S.

**🔧 Fitur /dev/random**
1. Membuat file random.c berisi fungsi randomread() yang menghasilkan byte acak.
2. Registrasi fungsi ini di array devsw[] pada file.c.
3. Menambahkan entri device /dev/random pada init.c menggunakan mknod().

---

## ✅ Uji Fungsionalitas

**Program chmodtest.c**
```
int fd = open("readonly.txt", O_CREATE | O_RDWR);
write(fd, "hello", 5);
close(fd);
chmod("readonly.txt", 1); // ubah jadi read-only
fd = open("readonly.txt", O_RDWR);
if (write(fd, "world", 5) < 0)
  printf("Write blocked as expected\n");
```
**Output**
```
Write blocked as expected
```

**Program randomtest.c**
```
int fd = open("/dev/random", O_RDONLY);
char buf[8];
read(fd, buf, 8);
for (int i = 0; i < 8; i++)
  printf("%d ", buf[i]);
printf("\n");
```
**Output**
```
73 12 201 88 45 39 111 6
```
---

## 📷Screenshot
**randomtest**

<img width="549" height="338" alt="randomtest" src="https://github.com/user-attachments/assets/26e391e0-c2c4-415e-bd4c-8666b60926ab" />

**chmodtest**

<img width="697" height="317" alt="chmodtest" src="https://github.com/user-attachments/assets/2a85f82a-0a7f-46bb-a46f-9a41c2625926" />

---

## ⚠️ Kendala yang Dihadapi

1. Nilai mode tidak disimpan ke disk, sehingga akan reset saat reboot.
2. Lupa menambahkan mknod("/dev/random", ...) menyebabkan open() gagal.
3. Salah indeks pada devsw[] membuat driver tidak aktif.
4. Fungsi acak menggunakan seed statis, sehingga output selalu sama tiap boot.
---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


