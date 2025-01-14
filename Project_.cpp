#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

struct Randevu {
    int id;
    string tarih;
    string saat;
    string kisi;
    string aciklama;
};

vector<Randevu> randevuListesi;

const string dosyaAdi = "randevular.txt";

bool tarihKontrol(const string& tarih) {
    if (tarih.length() != 10 || tarih[2] != '/' || tarih[5] != '/') return false;
    int gun = stoi(tarih.substr(0, 2));
    int ay = stoi(tarih.substr(3, 2));
    int yil = stoi(tarih.substr(6, 4));
    if (gun < 1 || gun > 31 || ay < 1 || ay > 12 || yil < 2023) return false;
    return true;
}


bool saatKontrol(const string& saat) {
    if (saat.length() != 5 || saat[2] != ':') return false;
    int hour = stoi(saat.substr(0, 2));
    int minute = stoi(saat.substr(3, 2));
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) return false;
    return true;
}


bool randevuCakismasiKontrol(const string& tarih, const string& saat) {
    for (const auto& randevu : randevuListesi) {
        if (randevu.tarih == tarih && randevu.saat == saat) {
            return true;
        }
    }
    return false;
}


void randevuEkle() {
    Randevu yeniRandevu;
    yeniRandevu.id = randevuListesi.empty() ? 1 : randevuListesi.back().id + 1;

    cout << "Tarih (GG/AA/YYYY): ";
    cin >> yeniRandevu.tarih;
    if (!tarihKontrol(yeniRandevu.tarih)) {
        cout << "Gecersiz tarih formatı!\n";
        return;
    }

    cout << "Saat (HH:MM): ";
    cin >> yeniRandevu.saat;
    if (!saatKontrol(yeniRandevu.saat)) {
        cout << "Gecersiz saat formatı!\n";
        return;
    }

    if (randevuCakismasiKontrol(yeniRandevu.tarih, yeniRandevu.saat)) {
        cout << "Bu tarih ve saatte zaten bir randevu var!\n";
        return;
    }

    cin.ignore(); 
    cout << "Kisi: ";
    getline(cin, yeniRandevu.kisi);
    cout << "Aciklama: ";
    getline(cin, yeniRandevu.aciklama);

    randevuListesi.push_back(yeniRandevu);
    cout << "Randevu basariyla eklendi!\n";
}

void randevuListele() {
    if (randevuListesi.empty()) {
        cout << "Kayitli randevu bulunmamaktadir.\n";
        return;
    }

    for (const auto& randevu : randevuListesi) {
        cout << "ID: " << randevu.id << "\n";
        cout << "Tarih: " << randevu.tarih << "\n";
        cout << "Saat: " << randevu.saat << "\n";
        cout << "Kisi: " << randevu.kisi << "\n";
        cout << "Aciklama: " << randevu.aciklama << "\n";
        cout << "--------------------------\n";
    }
}

void randevuGuncelle() {
    int id;
    cout << "Guncellenecek randevu ID'sini girin: ";
    cin >> id;

    for (auto& randevu : randevuListesi) {
        if (randevu.id == id) {
            string yeniTarih, yeniSaat;

            cout << "Yeni Tarih (GG/AA/YYYY): ";
            cin >> yeniTarih;
            if (!tarihKontrol(yeniTarih)) {
                cout << "Gecersiz tarih formatı!\n";
                return;
            }

            cout << "Yeni Saat (HH:MM): ";
            cin >> yeniSaat;
            if (!saatKontrol(yeniSaat)) {
                cout << "Gecersiz saat formatı!\n";
                return;
            }

            if (randevuCakismasiKontrol(yeniTarih, yeniSaat) && (yeniTarih != randevu.tarih || yeniSaat != randevu.saat)) {
                cout << "Bu tarih ve saatte zaten bir randevu var!\n";
                return;
            }

            randevu.tarih = yeniTarih;
            randevu.saat = yeniSaat;
            cin.ignore(); 
            cout << "Yeni Kisi: ";
            getline(cin, randevu.kisi);
            cout << "Yeni Aciklama: ";
            getline(cin, randevu.aciklama);

            cout << "Randevu basariyla guncellendi!\n";
            return;
        }
    }

    cout << "Belirtilen ID'ye sahip randevu bulunamadi.\n";
}


void randevuSil() {
    int id;
    cout << "Silinecek randevu ID'sini girin: ";
    cin >> id;

    for (auto it = randevuListesi.begin(); it != randevuListesi.end(); ++it) {
        if (it->id == id) {
            randevuListesi.erase(it);
            cout << "Randevu basariyla silindi!\n";
            return;
        }
    }

    cout << "Belirtilen ID'ye sahip randevu bulunamadi.\n";
}


void randevulariKaydet() {
    ofstream dosya(dosyaAdi);
    if (dosya.is_open()) {
        for (const auto& randevu : randevuListesi) {
            dosya << randevu.id << "," << randevu.tarih << "," << randevu.saat << "," << randevu.kisi << "," << randevu.aciklama << "\n";
        }
        dosya.close();
    }
    else {
        cout << "Randevular kaydedilirken bir hata olustu!\n";
    }
}


void randevulariYukle() {
    ifstream dosya(dosyaAdi);
    if (dosya.is_open()) {
        string satir;
        while (getline(dosya, satir)) {
            Randevu randevu;
            size_t pos = 0;
            string token;
            int field = 0;

            while ((pos = satir.find(',')) != string::npos) {
                token = satir.substr(0, pos);
                switch (field) {
                case 0: randevu.id = stoi(token); break;
                case 1: randevu.tarih = token; break;
                case 2: randevu.saat = token; break;
                case 3: randevu.kisi = token; break;
                }
                satir.erase(0, pos + 1);
                field++;
            }
            randevu.aciklama = satir;
            randevuListesi.push_back(randevu);
        }
        dosya.close();
    }
}

void randevuHatirlatici() {
    time_t simdi = time(0);
    tm* ltm = localtime(&simdi);
    string bugun = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);

    for (const auto& randevu : randevuListesi) {
        if (randevu.tarih == bugun) {
            cout << "Bugun " << randevu.saat << " saatinde " << randevu.kisi << " ile randevunuz var!\n";
            cout << "Aciklama: " << randevu.aciklama << "\n";
            cout << "--------------------------\n";
        }
    }
}

void menuGoster() {
    cout << "1. Randevu Ekle\n";
    cout << "2. Randevu Listele\n";
    cout << "3. Randevu Guncelle\n";
    cout << "4. Randevu Sil\n";
    cout << "5. Randevu Hatirlatici\n";
    cout << "6. Cikis\n";
    cout << "Seciminiz: ";
}

int main() {
    randevulariYukle();
    int secim;

    while (true) {
        menuGoster();
        cin >> secim;

        switch (secim) {
        case 1:
            randevuEkle();
            break;
        case 2:
            randevuListele();
            break;
        case 3:
            randevuGuncelle();
            break;
        case 4:
            randevuSil();
            break;
        case 5:
            randevuHatirlatici();
            break;
        case 6:
            randevulariKaydet();
            cout << "Programdan cikiliyor...\n";
            return 0;
        default:
            cout << "Gecersiz secim! Tekrar deneyin.\n";
        }
    }

    return 0;
}