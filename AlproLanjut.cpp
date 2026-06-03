#include <iostream>
#include <fstream>
using namespace std;

struct Film {
    int idFilm;
    string judul;
    string genre;
    int harga;
};

struct Pemesanan {
    int idPesanan = 0;
    string nama = "";
};

struct Bioskop{
    Film Tayang;
    Pemesanan Kursi[5][6];
    string jam;
};

void lihat_film_dan_kursi () {
    
}

Bioskop Ruang[3];

int main (){
    ifstream file("Data/Film/Data_film.txt")
    if (!file.is_open()) {
        cout << "Data film tidak ada: Tanyakan manager kenapa tidak ada filem yang ditanyakan";
        return 0;
    }

    file.close();

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
                break;
            case 2 :
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
