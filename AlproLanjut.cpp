#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // Ditambahkan untuk parsing string dari file
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
            ofstream fileOut("Data/pesanan.txt", ios :: app);
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

void lihatDataPesanan() {
    ifstream fileIn("Data/pesanan.txt");
    
    // Cek apakah file bisa dibuka (apakah sudah ada pesanan sebelumnya)
    if (!fileIn.is_open()) {
        cout << "\n[!] Belum ada data pesanan atau file tidak ditemukan.\n\n";
        return;
    }

    string line;
    cout << "\n+==== DATA RIWAYAT PESANAN ====+\n";
    
    bool adaData = false;
    
    // Membaca file baris demi baris
    while (getline(fileIn, line)) {
        stringstream ss(line);
        string tempIndex, tempR, tempC, tempId, nama;

        // Memisahkan data berdasarkan delimiter '|'
        getline(ss, tempIndex, '|');
        getline(ss, tempR, '|');
        getline(ss, tempC, '|');
        getline(ss, tempId, '|');
        getline(ss, nama);

        // Memastikan baris tidak kosong sebelum diproses
        if (!tempIndex.empty()) {
            adaData = true;
            
            // Konversi string ke integer untuk index
            int idx = stoi(tempIndex);
            int barisKursi = stoi(tempR) + 1; // +1 karena saat disimpan formatnya index (0-4)
            int kolomKursi = stoi(tempC) + 1; // +1 karena saat disimpan formatnya index (0-5)
            
            // Ambil data film berdasarkan index ruang
            string judulFilm = tayang[idx].judul;
            string hargaTiket = tayang[idx].harga;

            // Tampilkan ke layar
            cout << "ID Pesanan : " << tempId << "\n";
            cout << "Nama       : " << nama << "\n";
            cout << "Film       : " << judulFilm << " (Ruang " << (idx + 1) << ")\n";
            cout << "Kursi      : Baris " << barisKursi << ", Kolom " << kolomKursi << "\n";
            cout << "Harga      : Rp" << hargaTiket << "\n";
            cout << "-----------------------------------\n";
        }
    }
    
    if (!adaData) {
        cout << "Data pesanan masih kosong.\n";
    }
    
    cout << "\n";
    fileIn.close();
}

void cariPesanan() {
    int idCari;
    cout << "\n+==== CARI DATA PESANAN ====+\n";
    cout << "Masukkan ID Pesanan yang dicari: ";
    cin >> idCari;

    bool ditemukan = false;

    // Looping untuk mencari ke seluruh ruang, baris, dan kolom kursi
    for (int i = 0; i < 3; i++) {
        for (int baris = 0; baris < 5; baris++) {
            for (int kolom = 0; kolom < 6; kolom++) {
                
                // Jika ID pada kursi cocok dengan ID yang dicari
                if (ruang[i].kursi[baris][kolom].idPesanan == idCari) {
                    cout << "\n PESANAN DITEMUKAN! \n";
                    cout << "-----------------------------------\n";
                    cout << "ID Pesanan  : " << idCari << "\n";
                    cout << "Nama Pemesan: " << ruang[i].kursi[baris][kolom].nama << "\n";
                    cout << "Film        : " << tayang[i].judul << " (Ruang " << (i + 1) << ")\n";
                    cout << "Posisi Kursi: Baris " << (baris + 1) << ", Kolom " << (kolom + 1) << "\n";
                    cout << "-----------------------------------\n\n";
                    
                    ditemukan = true;
                    break; // Hentikan loop kolom jika sudah ketemu
                }
            }
            if (ditemukan) break; // Hentikan loop baris
        }
        if (ditemukan) break; // Hentikan loop ruang
    }

    // Jika setelah semua dicek tidak ada yang cocok
    if (!ditemukan) {
        cout << "\n Pesanan dengan ID " << idCari << " tidak ditemukan atau belum terdaftar.\n\n";
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
				lihatDataPesanan();
                break;
            case 4 :
				cariPesanan();
                break;
            case 5 :
                break;
            default :
                cout <<"Opsi tidak ditemukan"<<endl;
                break;
        }
    } while (pilihan_menu != 5);
}
