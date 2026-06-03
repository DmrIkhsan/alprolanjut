#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Film {
    string idFilm;
    string judul;
    string genre;
    string harga;
};

struct Pemesanan {
    int idPesanan = 0;
    string nama = "";
};

struct Bioskop {
    string idFilmTayang;
    Pemesanan kursi[5][6];
    string jam;
};

Bioskop ruang[3];
Film tayang[3];

int counterPesanan = 1000; // Untuk menghasilkan ID Pesanan unik otomatis (1001, 1002, dst)

// Fungsi rekursif untuk menghitung total harga (jika beli 'n' tiket)
int hitungTotalRekursif(int jumlahTiket, int hargaSatuan) {
    if (jumlahTiket <= 1) {
        return hargaSatuan;
    }
    return hargaSatuan + hitungTotalRekursif(jumlahTiket - 1, hargaSatuan);
}

// ==== FUNGSI BARU: Membaca data pesanan saat program dibuka ====
void muatDataPesanan() {
    ifstream fileIn("Data/pesanan.txt");
    if (!fileIn.is_open()) {
        return; // Jika file belum ada, abaikan (berarti belum ada pesanan sama sekali)
    }

    string line;
    int maxId = 1000; // Untuk melacak ID pesanan tertinggi

    // Format di file: indexRuang|baris|kolom|idPesanan|namaPemesan
    while (getline(fileIn, line)) {
        stringstream ss(line);
        string tempIndex, tempR, tempC, tempId, nama;

        getline(ss, tempIndex, '|');
        getline(ss, tempR, '|');
        getline(ss, tempC, '|');
        getline(ss, tempId, '|');
        getline(ss, nama);

        if (!tempIndex.empty()) {
            int idx = stoi(tempIndex);
            int r = stoi(tempR);
            int c = stoi(tempC);
            int idPes = stoi(tempId);

            // Masukkan kembali ke matriks bioskop
            ruang[idx].kursi[r][c].nama = nama;
            ruang[idx].kursi[r][c].idPesanan = idPes;

            // Perbarui maxId jika menemukan ID yang lebih besar
            if (idPes > maxId) {
                maxId = idPes;
            }
        }
    }
    
    counterPesanan = maxId; // Lanjutkan ID pesanan dari ID terakhir
    fileIn.close();
}

void pesanTiketLaluKirim() {
    cin.ignore(); // WAJIB: Membersihkan sisa 'Enter' dari menu utama (cin >> pilihan_menu)
    
    string namaPemesan;
    cout << "\n+==== FORM PEMESANAN TIKET ====+\n";
    cout << "Masukkan Nama Anda: ";
    getline(cin, namaPemesan);

    // 1. Pilih Ruang/Film
    cout << "\nDaftar Film & Ruang yang Tersedia:\n";
    for (int i = 0; i < 3; i++) {
        cout << (i + 1) << ". Ruang " << (i + 1) << " - " << tayang[i].judul << " (Rp" << tayang[i].harga << ")\n";
    }
    int pilihanRuang;
    cout << "Pilih nomor Ruang (1-3): ";
    cin >> pilihanRuang;

    if (pilihanRuang < 1 || pilihanRuang > 3) {
        cout << "❌ Pilihan ruang tidak valid!\n\n";
        return;
    }
    int index = pilihanRuang - 1;

    // 2. Tampilkan Denah Kursi agar User Bisa Memilih
    cout << "\n+==== SILAHKAN PILIH KURSI (RUANG " << pilihanRuang << ") ====+\n";
    cout << "          [ LAYAR BIOSKOP ]\n\n";
    for (int baris = 0; baris < 5; baris++) {
        for (int kolom = 0; kolom < 6; kolom++) {
            if (ruang[index].kursi[baris][kolom].nama == "") {
                cout << "[" << (baris + 1) << "." << (kolom + 1) << "] ";
            } else {
                cout << "[ X ] "; // X = Sudah dipesan
            }
        }
        cout << "\n";
    }

    // 3. Input Koordinat Kursi
    int b, k;
    cout << "\nMasukkan nomor Baris kursi (1-5): ";
    cin >> b;
    cout << "Masukkan nomor Kolom kursi (1-6): ";
    cin >> k;

    // Validasi batas matriks 5x6
    if (b >= 1 && b <= 5 && k >= 1 && k <= 6) {
        int r = b - 1; // Ubah ke index array (0-4)
        int c = k - 1; // Ubah ke index array (0-5)

        // 4. Cek apakah kursi sudah terisi
        if (ruang[index].kursi[r][c].nama == "") {
            counterPesanan++; // Naikkan ID pesanan
            
            // Simpan data pemesan ke matriks kursi bioskop
            ruang[index].kursi[r][c].nama = namaPemesan;
            ruang[index].kursi[r][c].idPesanan = counterPesanan;

            // Konversi string harga ke int untuk fungsi rekursif
            int hargaInt = stoi(tayang[index].harga); 
            int totalBayar = hitungTotalRekursif(1, hargaInt); // Memanggil fungsi rekursif (1 tiket)

            // ==== MENYIMPAN DATA KE TXT (APPEND) ====
            // Mode ios::app (append) akan menambah teks ke baris baru tanpa menghapus isi lama
            ofstream fileOut("Data/pesanan.txt");
            if (fileOut.is_open()) {
                // Format: indexRuang|baris|kolom|idPesanan|namaPemesan
                fileOut << index << "|" << r << "|" << c << "|" << counterPesanan << "|" << namaPemesan << "\n";
                fileOut.close();
            } else {
                cout << "Gagal menyimpan ke pesanan.txt!\n";
            }

            cout << "\n PEMESANAN TIKET BERHASIL! \n";
            cout << "-----------------------------------\n";
            cout << "ID Pesanan  : " << counterPesanan << "\n";
            cout << "Nama        : " << namaPemesan << "\n";
            cout << "Film        : " << tayang[index].judul << "\n";
            cout << "Kursi       : Baris " << b << ", Kolom " << k << "\n";
            cout << "Total Bayar : Rp" << totalBayar << "\n";
            cout << "-----------------------------------\n\n";
        } else {
            cout << " Maaf, kursi nomor [" << b << "." << k << "] sudah di-booking orang lain!\n\n";
        }
    } else {
        cout << " Posisi kursi tidak tersedia di bioskop ini!\n\n";
    }
}

void lihat_film_dan_kursi () {
    cout << "\n+==== DAFTAR FILM TAYANG ===+\n";
    for (int i = 0; i < 3; i++) {
        cout << "Ruang " << (i + 1) << "\n";
        cout << "ID Film : " << tayang[i].idFilm << "\n";
        cout << "Judul   : " << tayang[i].judul << "\n";
        cout << "Genre   : " << tayang[i].genre << "\n";
        cout << "Harga   : Rp" << tayang[i].harga << "\n";
        cout << "-----------------------------\n";
    }

    int pilihanRuang;
    cout << "Pilih nomor Ruang (1-3) untuk melihat denah kursi: ";
    cin >> pilihanRuang;

    // Validasi input ruang
    if (pilihanRuang >= 1 && pilihanRuang <= 3) {
        int index = pilihanRuang - 1; // Array dimulai dari 0
        
        cout << "\n+==== DENAH KURSI RUANG " << pilihanRuang << " ===+\n";
        cout << "Film: " << tayang[index].judul << "\n\n";
        cout << "        [ LAYAR BIOSKOP ]\n\n";
        
        // Looping untuk menampilkan matriks 5 baris x 6 kolom
        for (int baris = 0; baris < 5; baris++) {
            for (int kolom = 0; kolom < 6; kolom++) {
                // Mengecek apakah kursi sudah dipesan (nama tidak kosong)
                if (ruang[index].kursi[baris][kolom].nama == "") {
                    cout << "[ O ] "; // O = Kosong
                } else {
                    cout << "[ X ] "; // X = Terisi
                }
            }
            cout << "\n"; // Pindah ke baris kursi berikutnya
        }
        cout << "\nKeterangan: [ O ] = Kosong, [ X ] = Terisi\n\n";
    } else {
        cout << "Pilihan ruang tidak valid!\n\n";
    }
}

int main () {
    // 1. Load data film
    for(int i = 0; i < 3; i++){
        string namaFile = "Data/Film/Data_film"+to_string(i)+".txt";
        ifstream file(namaFile);
        if (!file.is_open()) {
            cout << "Data film tidak ada: Tanyakan manager kenapa tidak ada filem yang ditanyakan\n";
            return 0; // Hentikan program jika data film tidak lengkap
        }
        getline(file, tayang[i].idFilm);
        getline(file, tayang[i].judul);
        getline(file, tayang[i].genre);
        getline(file, tayang[i].harga);
        ruang[i].idFilmTayang = tayang[i].idFilm;

        file.close();
    }
    
    // 2. Load riwayat pemesanan agar kursi kembali terisi
    muatDataPesanan();
    
    int pilihan_menu;
    do {
        cout << "+==== PEMESANAN TIKET BIOSKOP ====+\n";
        cout << "1. Lihat Film dan Kursi\n"; 
        cout << "2. Pesan Tiket\n"; 
        cout << "3. Lihat Data Pesanan\n"; 
        cout << "4. Cari Pesanan\n"; 
        cout << "5. Keluar\n";
        cout << "Pilih menu (1-5) : ";
        cin >> pilihan_menu;

        switch (pilihan_menu) {
            case 1 :
                lihat_film_dan_kursi ();
                break;
                
            case 2 :
                pesanTiketLaluKirim ();
                break;
            case 3 :
                break;
            case 4 :
                break;
            case 5 :
                break;
            default :
                cout <<"Opsi tidak ditemukan"<<endl;
                break;
        }
    } while (pilihan_menu != 5);
}
