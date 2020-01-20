
#include "operacije_nad_datotekama.h"

int main()
{
    FILE *fajlSerijski = NULL;
    FILE *fajlSekvencijalni = NULL;
    int running = 1;

    while (running) {
        int userInput;
        printf("1. Kreiraj datoteku (Serijsku) \n");
        printf("2. Otvori datoteku.\n");
        printf("3. Dodaj pregled pacijenta. \n");
        printf("4. Ispisi slogove. \n");
        printf("5. Pronadjite zeljene podatke o pregledu. \n");
        printf("6. Izmeni podatke o zeljenom pregledu. \n");
        printf("7. Obrisi logicki zeljeni slog. \n");
        printf("8. Fizicki obrisi slog iz serijske datoteke. \n");
        printf("9. Upisi u sekvencijalnu sumu pregleda. \n");
        printf("0: Izlazak iz menija.\n");

        if (fajlSerijski == NULL) {
            printf("NAPOMENA: Datoteka nije otvorena!\n\n");
        }
        scanf("%d", &userInput);
        getc(stdin);

        switch (userInput) {
        case 0:
        {
            if (fajlSerijski != NULL) {
                fclose(fajlSerijski);
            }
            if (fajlSekvencijalni != NULL) {
                fclose(fajlSekvencijalni);
            }

            running = 0;
            break;
        }

        case 1:
        {
            char filename[20];
            printf("Unesite naziv datoteke za kreiranje: ");
            scanf("%s", &filename[0]);
            kreirajDatoteku(filename);
            printf("\n");
            break;
        }
        case 2:
        {
            printf("Unesite naziv datoteke za otvaranje: ");
            char filename[20];
            scanf("%s", filename);
            fajlSerijski = otvoriDatoteku(filename);
            printf("\n");
            break;
        }

        case 3:
        {
            SLOG slog;
            printf("Unesite sifru pregleda: ");
            scanf("%s", slog.sifraPregleda);
            printf("Unesite broj kartona: ");
            scanf("%s", slog.brojKartona);
            printf("Unesite ime pacijenta: ");
            scanf("%s", slog.imePacijenta);
            printf("Unesite ime doktora: ");
            scanf("%s", slog.imeDoktora);
            printf("Unesite duzinu trajanja pregleda: ");
            scanf("%d", &slog.duzinaTrajanjaPregleda);
            slog.deleted = 0;

            dodajSlogUSerijsku(fajlSerijski, &slog);
            printf("\n");
            break;
        }
        case 4:
        {
            ispisiSlogove(fajlSerijski);
            break;
        }

        case 5:
        {
            char *sifraPregleda[6];
            printf("Unesite sifru pregleda za pretragu: ");
            scanf("%s", sifraPregleda);
            SLOG *slog;
            slog = pronadjiSlog(fajlSerijski, sifraPregleda);
            printf("Sifra pregleda\tBroj kartona\tIme pacijenta\tIme doktora\tDuzina pregleda\n");
            ispisiSlog(slog);
            printf("\n");
            break;

        }
        case 6:
        {
            printf("Unesite sifru pregleda koju zelite da izmenite: ");
            char *sifraPregleda[6];
            scanf("%s", sifraPregleda);
            printf("Unesite broj kartona: ");
            char *brojKartona[6];
            scanf("%s", brojKartona);
            printf("Unesite trajanje pregleda: ");
            int duzinaPregleda;
            scanf("%d", &duzinaPregleda);
            azurirajSlog(fajlSerijski, sifraPregleda, brojKartona, duzinaPregleda);
            printf("\n");
            break;
        }

        case 7:
        {
            printf("Unesite sifru pregleda za logicko brisanje: ");
            char *sifraPregleda[6];
            scanf("%s", sifraPregleda);
            logickiObrisiSlog(fajlSerijski, sifraPregleda);
            printf("\n");
            break;
        }

        case 8:
        {
            printf("Unesite sifru pregleda za fizicko brisanje: ");
            char *sifraPregleda[6];
            scanf("%s", sifraPregleda);
            fizickiObrisiSlog(fajlSerijski, sifraPregleda);
            printf("\n");
            break;
        }

        case 9:
        {
            printf("Unesite naziv za kreiranje sekvencijalne datoteke: ");
            char *filename[20];
            scanf("%s", filename);
            kreirajSekvencijalnuDatoteku(filename);
            fajlSekvencijalni = otvoriDatoteku(filename);
            upisiUSekvencijalnu(fajlSerijski, fajlSekvencijalni);
            ispisiSveSekv(fajlSekvencijalni);
            printf("\n");
            break;

        }
        }
    }
}
