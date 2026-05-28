#include <iostream>
#include <fstream>
using namespace std;

void lihat_film_dan_kursi () {
    
}

int main (){
    int pilihan_menu;
    do {
        cout << "+==== PEMESANAN TIKET BIOSKOP ====+\n";
        cout << "1. Lihat Film dan Kursi\n";
        cout << "2. Pesan Tiket+\n";
        cout << "3. Lihat Data Pesanan\n";
        cout << "4. Cari Pesanan\n";
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
                break;
        }
    } while (pilihan_menu != 5);
}