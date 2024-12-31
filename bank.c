#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct MusteriP {
    char isim[50];
    char telNo[11];
} Musteri;

typedef struct HesapS {
    Musteri sahibi;
    char no[6];
    char parola[10];
    float bakiye;
} Hesap;

Hesap olusturHesap(const char *hesapSahibiIsmi, const char *telno, const char *HesapNo, const char *HesapParola) {
    Musteri tmpMusteri; 
    Hesap tmpHesap;
    strncpy(tmpMusteri.isim, hesapSahibiIsmi, sizeof(tmpMusteri.isim) - 1);
    strncpy(tmpMusteri.telNo, telno, sizeof(tmpMusteri.telNo) - 1);
    tmpHesap.sahibi = tmpMusteri;
    strncpy(tmpHesap.no, HesapNo, sizeof(tmpHesap.no) - 1);
    strncpy(tmpHesap.parola, HesapParola, sizeof(tmpHesap.parola) - 1);
    tmpHesap.bakiye = 0.0; 
    return tmpHesap;
}

void paraCek(Hesap *h) {
    float cekilecekTutar;
    printf("Cekmek istediginiz tutari giriniz: ");
    if (scanf("%f", &cekilecekTutar) != 1 || cekilecekTutar <= 0) {
        printf("Gecersiz miktar.");
        return;
    }
    if (cekilecekTutar > h->bakiye) {
        printf("Yetersiz bakiye.\n");
    } else {
        h->bakiye -= cekilecekTutar;
        printf("Para cekme islemi basarili. Guncel bakiye = %.2f\n", h->bakiye);
    }
}

void paraYatir(Hesap *h) {
    float yatirilanTutar;
    printf("Yatirmak istediginiz tutari giriniz: ");
    if (scanf("%f", &yatirilanTutar) != 1 || yatirilanTutar <= 0) {
        printf("Gecersiz miktar.\n");
        return;
    }
    h->bakiye += yatirilanTutar;
    printf("Para yatirma islemi basarili. Guncel bakiye = %.2f\n", h->bakiye);
}

void faturaOde(Hesap *h) {
    srand(time(NULL));
    int fatura = (rand() % 901) + 100;
    printf("Faturaniz %d TL'dir. Odemeyi onaylamak icin 1'e, ana menuye donmek icin 0'a basiniz: ", fatura);
    int onay;
    while (1) {
        if (scanf("%d", &onay) != 1 || (onay != 0 && onay != 1)) {
            printf("Lutfen 1 veya 0 giriniz.\n");
            continue;
        }
        if (onay == 1) {
            if (h->bakiye < fatura) {
                printf("Yetersiz bakiye.\n");
            } else {
                h->bakiye -= fatura;
                printf("Fatura odenmistir. Guncel bakiye = %.2f\n", h->bakiye);
            }
        } else {
            printf("Ana menuye donuluyor...\n");
        }
        break;
    }
}

void bakiyeSorgula(Hesap *h) {
    printf("Guncel Bakiyeniz = %.2f\n", h->bakiye);
}

void hesapKaydet(Hesap *hesaplar, int sayi) {
    FILE *dosya = fopen("hesaplar.txt", "w");
    if (dosya != NULL) {
        fwrite(&sayi, sizeof(int), 1, dosya);
        fwrite(hesaplar, sizeof(Hesap), sayi, dosya);
        fclose(dosya);
    } else {
        printf("Hesap verileri kaydedilemedi.\n");
    }
}

int hesapYukle(Hesap *hesaplar) {
    FILE *dosya = fopen("hesaplar.txt", "r");
    int sayi = 0;
    if (dosya != NULL) {
        fread(&sayi, sizeof(int), 1, dosya);
        fread(hesaplar, sizeof(Hesap), sayi, dosya);
        fclose(dosya);
    }
    return sayi;
}

void yeniKullaniciEkle(Hesap *hesaplar, int *hesapSayisi) {
    char isim[50], telNo[11], hesapNo[6], parola[10];

    printf("Yeni hesap olusturuluyor...\n");
    printf("Isim: ");
    scanf("%49s", isim);
    printf("Telefon numarasi: ");
    scanf("%10s", telNo);
    printf("Hesap numarasi: ");
    scanf("%5s", hesapNo);
    printf("Parola: ");
    scanf("%9s", parola);

    Hesap yeniHesap = olusturHesap(isim, telNo, hesapNo, parola);
    hesaplar[*hesapSayisi] = yeniHesap;
    (*hesapSayisi)++;
    printf("Yeni hesap basariyla olusturuldu.\n");

    hesapKaydet(hesaplar, *hesapSayisi);
}

int kullaniciDogrula(Hesap *hesaplar, int hesapSayisi, char *no, char *parola) {
    for (int i = 0; i < hesapSayisi; i++) {
        if (strcmp(hesaplar[i].no, no) == 0 && strcmp(hesaplar[i].parola, parola) == 0) {
            return i; 
        }
    }
    return -1; 
}

int menuIslemleri(Hesap *hesap, Hesap *hesaplar, int *hesapSayisi) {
    while (1) {
        printf("\nYapmak istediginiz islemi seciniz:\n");
        printf("1 = Bakiye sorgula\n2 = Para yatir\n3 = Para cek\n4 = Fatura ode\n5 = oturumu kapat \n6= Cikis yap \n");
        int islem;
        if (scanf("%d", &islem) != 1) {
            printf("Gecersiz secim.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (islem) {
            case 1:
                bakiyeSorgula(hesap);
                break;
            case 2:
                paraYatir(hesap);
                break;
            case 3:
                paraCek(hesap);
                break;
            case 4:
                faturaOde(hesap);
                break;
            case 5:
                printf("hesaptan cikiliyo \n");
                return 0;
            case 6:
                printf("Cikis yapiliyor.\n");
                hesapKaydet(hesaplar, *hesapSayisi);
                return 5;
                break;
            default:
                printf("Gecersiz secim.\n");
        }
    }
}

int main() {
    srand(time(NULL)); // Sadece bir kez random seed
    const int maxKisi = 100;
    Hesap bankaHesaplari[maxKisi];
    int hesapSayisi = hesapYukle(bankaHesaplari);

    printf("Yeni hesap olusturmak ister misiniz? (Evet icin 'E', devam etmek icin herhangi bir tusa basin): ");
    char secim;
    scanf(" %c", &secim);

    if (secim == 'E' || secim == 'e') {
        yeniKullaniciEkle(bankaHesaplari, &hesapSayisi);
    }

    while (1) {
        char no[6], parola[10];
        printf("\nHesap numaranizi giriniz: ");
        scanf("%5s", no);
        printf("Parolanizi giriniz: ");
        scanf("%9s", parola);

        int index = kullaniciDogrula(bankaHesaplari, hesapSayisi, no, parola);
        if (index != -1) {
            printf("Girisi basarili.\n");
           int dur= menuIslemleri(&bankaHesaplari[index], bankaHesaplari, &hesapSayisi);
           if(dur==5)
           {
            break;;
           }
        } else {
            printf("Hesap numarasi veya parola yanlis. Lutfen tekrar deneyiniz.\n");
        }
    }

    return 0;
}
