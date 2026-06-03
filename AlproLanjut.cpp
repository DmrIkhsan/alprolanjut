#include <iostream>
#include <fstream>
#include <string>
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

struct Bioskop{
    string idFilmTayang;
    Pemesanan kursi[5][6];
    string jam;
};

Bioskop ruang[3];
Film tayang[3];

Pemesanan daftarPesanan[100];
int jumlahPesanan = 0;

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

//Menu 3
void lihat_data_pesanan() {
    if (jumlahPesanan == 0) {
        cout << "Belum ada pesanan.\n";
        return;
    }
    for (int i = 0; i < jumlahPesanan - 1; i++) {
        for (int j = 0; j < jumlahPesanan - i - 1; j++) {
            if (daftarPesanan[j].idPesanan >
                daftarPesanan[j + 1].idPesanan) {

                Pemesanan temp = daftarPesanan[j];
                daftarPesanan[j] = daftarPesanan[j + 1];
                daftarPesanan[j + 1] = temp;
            }
        }
    }
    cout << "\n===== DATA PESANAN =====\n";
    for (int i = 0; i < jumlahPesanan; i++) {
        cout << "ID Pesanan : "
             << daftarPesanan[i].idPesanan << endl;
        cout << "Nama       : "
             << daftarPesanan[i].nama << endl;
        cout << "--------------------------\n";
    }
}

int main (){
	for(int i = 0; i < 3; i++){
		string namaFile = "Data/Film/Data_film"+to_string(i)+".txt";
		ifstream file(namaFile);
		if (!file.is_open()) {
			cout << "Data film tidak ada: Tanyakan manager kenapa tidak ada filem yang ditanyakan";
			return 0;
		}
		getline(file, tayang[i].idFilm);
		getline(file, tayang[i].judul);
		getline(file, tayang[i].genre);
		getline(file, tayang[i].harga);
		ruang[i].idFilmTayang = tayang[i].idFilm;

		file.close();
	}
	
    int pilihan_menu;
    do {
        cout << "+==== PEMESANAN TIKET BIOSKOP ====+\n";
        cout << "1. Lihat Film dan Kursi\n"; //menampilkan daftar film dan dena kursi bioskop (array)
        cout << "2. Pesan Tiket+\n"; //input nama, pilih film & kursi, total harga tiket (rekursif), dll
        cout << "3. Lihat Data Pesanan\n"; //hanya admin. menampilkan data (sorting)
        cout << "4. Cari Pesanan\n"; //hanya admin. (searching)
        cout << "5. Keluar\n";
        cout << "Pilih menu (1-5) : ";
        cin >> pilihan_menu;

        switch (pilihan_menu) {
            case 1 :
				lihat_film_dan_kursi ();
                break;
            case 2 :
                break;
            case 3 :
				lihat_data_pesanan ();
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
