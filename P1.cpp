#include <iostream>
using namespace std;

class Buku {
   private:
    string nama;
    string peminjam = "";
    bool buku = true;

   public:
    Buku() {
        cout << "Sebuah buku akan ditambahkan di perpustakaan" << endl;
    }
    Buku(string s) {
        nama = s;
        cout << "Sebuah buku bernama " << nama << " akan ditambahkan di perpustakaan" << endl;
    }

    void borrow() {
        if (buku) {
            cout << "Berhasil meminjam buku\n";
            buku = false;
        } else {
            cout << "Gagal meminjam buku\n";
        }
    }

    void borrow(string s) {
        if (buku) {
            cout << s << " meminjam buku " << nama << endl;
            buku = false;
            peminjam = s;
        } else {
            cout << "Buku sedang dipinjam " << peminjam << endl;
        }
    }

    void kembalikan() {
        if (!buku) {
            buku = true;
            cout << "Buku dikembalikan " << peminjam << endl;
        } else {
            cout << "Tidak ada buku untuk dikembalikan" << endl;
        }
    }
};

int main() {
    Buku kalkulus;
    Buku fisika("Fisika Listrik");

    fisika.borrow();
    fisika.borrow();
    fisika.kembalikan();
    fisika.kembalikan();
    fisika.borrow("faiq");
    fisika.borrow("budi");
    // fisika.kembalikan();
}