#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOYUT = 8;
const int MAYIN_SAYISI = 10;

struct Hucre {
    bool mayin;
    bool acik;
    int cevreMayin;
};

void tahtaYaz(const vector<vector<Hucre>>& tahta, bool oyunBitti) {
    cout << "   ";
    for (int i = 0; i < BOYUT; i++) cout << i << " ";
    cout << endl;
    for (int i = 0; i < BOYUT; i++) {
        cout << i << "  ";
        for (int j = 0; j < BOYUT; j++) {
            if (tahta[i][j].acik) {
                if (tahta[i][j].mayin)
                    cout << "* ";
                else if (tahta[i][j].cevreMayin > 0)
                    cout << tahta[i][j].cevreMayin << " ";
                else
                    cout << ". ";
            } else {
                if (oyunBitti && tahta[i][j].mayin)
                    cout << "* ";
                else
                    cout << "# ";
            }
        }
        cout << endl;
    }
}

void cevreHesapla(vector<vector<Hucre>>& tahta) {
    for (int i = 0; i < BOYUT; i++) {
        for (int j = 0; j < BOYUT; j++) {
            if (tahta[i][j].mayin) continue;
            int say = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < BOYUT && ny >= 0 && ny < BOYUT)
                        if (tahta[nx][ny].mayin) say++;
                }
            }
            tahta[i][j].cevreMayin = say;
        }
    }
}

void acikHucreleriAc(vector<vector<Hucre>>& tahta, int x, int y) {
    if (x < 0 || x >= BOYUT || y < 0 || y >= BOYUT) return;
    if (tahta[x][y].acik) return;
    tahta[x][y].acik = true;
    if (tahta[x][y].cevreMayin == 0 && !tahta[x][y].mayin) {
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
                if (dx != 0 || dy != 0)
                    acikHucreleriAc(tahta, x + dx, y + dy);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    vector<vector<Hucre>> tahta(BOYUT, vector<Hucre>(BOYUT, {false, false, 0}));

    // Mayinlari yerlestir
    int yerlestirilen = 0;
    while (yerlestirilen < MAYIN_SAYISI) {
        int x = rand() % BOYUT;
        int y = rand() % BOYUT;
        if (!tahta[x][y].mayin) {
            tahta[x][y].mayin = true;
            yerlestirilen++;
        }
    }

    cevreHesapla(tahta);

    bool oyunBitti = false;
    bool kazandin = false;
    int acikHucreSayisi = 0;

    while (!oyunBitti) {
        tahtaYaz(tahta, false);
        cout << "Satir ve sutun gir (ornegin: 3 4): ";
        int x, y;
        cin >> x >> y;
        if (x < 0 || x >= BOYUT || y < 0 || y >= BOYUT) {
            cout << "Gecersiz konum!" << endl;
            continue;
        }
        if (tahta[x][y].acik) {
            cout << "Bu hucre zaten acik!" << endl;
            continue;
        }
        if (tahta[x][y].mayin) {
            oyunBitti = true;
            tahta[x][y].acik = true;
            tahtaYaz(tahta, true);
            cout << "Mayina bastin! Oyun bitti." << endl;
            break;
        } else {
            acikHucreleriAc(tahta, x, y);
        }

        // Kazanma kontrolu
        acikHucreSayisi = 0;
        for (int i = 0; i < BOYUT; i++)
            for (int j = 0; j < BOYUT; j++)
                if (tahta[i][j].acik) acikHucreSayisi++;
        if (acikHucreSayisi == BOYUT * BOYUT - MAYIN_SAYISI) {
            oyunBitti = true;
            kazandin = true;
            tahtaYaz(tahta, true);
            cout << "Tebrikler! Tum mayinsiz hucreleri actin ve kazandin!" << endl;
        }
    }

    cout << "Iyi denemeydi kanki. Bir daha beklerim." << endl;
    return 0;
}