#include "operacije_nad_datotekama.h"

void kreirajDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "wb");

    if (fajl == NULL) {
        printf("Error: Nemoguce kreirati datoteku!\n");
    } else {
        printf("Uspesno kreirana datoteka!");
        BLOK blok;
        fseek(fajl, 0, SEEK_SET);
        strcpy(blok.slogovi[0].sifraPregleda, OZNAKA_KRAJA_DATOTEKE);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
    }

    fclose(fajl);
}

FILE *otvoriDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "rb+");

    if (fajl == NULL) {
        printf("Error: Nemoguce otvoriti datoteku!\n");
    } else {
        printf("Uspesno otvorena datoteka!\n");
    }

    return fajl;
}

void kreirajSekvencijalnuDatoteku(char *filename) {
    FILE *fajl = fopen(filename, "wb");
    if (fajl == NULL) {
        printf("Error: Nemoguce kreirati datoteku!\n");
    } else {
        BLOKS blok;
        strcpy(blok.slogovi[0].brojKartona, OZNAKA_KRAJA_DATOTEKE);
        fwrite(&blok, sizeof(BLOKS), 1, fajl);
        printf("Uspesno otvorena sekvencijalna datoteka!\n");
        printf("%s", blok.slogovi[0].brojKartona);
    }

    fclose(fajl);
}

void dodajSlogUSerijsku(FILE *fajl, SLOG *slog) {
    if (fajl == NULL) {
        printf("Error: Datoteka verovatno nije otvorena!\n");
        return;
    }

    BLOK blok;
    fseek(fajl, -sizeof(BLOK), SEEK_END);

    while (fread(&blok, sizeof(BLOK), 1, fajl)) {
        int i;
        for (i = 0; i < FAKTOR_BLOKIRANJA; i++) {

            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
                break;
            }
        }
        i++;
        if (i < FAKTOR_BLOKIRANJA) {
            strcpy(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE);
            fseek(fajl, -sizeof(BLOK), SEEK_END);
            fwrite(&blok, sizeof(BLOK), 1, fajl);
            printf("Unet je novi slog!\n");
            return;
        } else {
            fseek(fajl, -sizeof(BLOK), SEEK_END);
            fwrite(&blok, sizeof(BLOK), 1, fajl);
            BLOK noviBlok;
            strcpy(noviBlok.slogovi[0].sifraPregleda, OZNAKA_KRAJA_DATOTEKE);
            fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
            printf("Unet je novi slog i napravljen je novi blok!\n");
            return;
        }
    }
}

void ispisiSlogove(FILE *fajl) {

    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    int rbBloka = 1;
    printf("BLOK SLOG\tSifra pregleda\tBroj kartona\tIme pacijenta\tIme doktora\tDuzina pregleda\n");
    while (fread(&blok, sizeof(BLOK), 1, fajl)) {

        for (int i = 0; i <FAKTOR_BLOKIRANJA; i++) {
            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("B%d S%d\t*", rbBloka, i);
                printf("\n");
                return;
            }
            else {
                if (!blok.slogovi[i].deleted) {
                    printf("B:%d S%d\t", rbBloka, i);
                    ispisiSlog(&blok.slogovi[i]);
                    printf("\n");
                }
            }
        }

        rbBloka++;
    }
}

void ispisiSlog(SLOG *slog) {
    printf("%s\t\t%s\t\t%s\t\t%s\t\t%d",
           slog->sifraPregleda,
           slog->brojKartona,
           slog->imePacijenta,
           slog->imeDoktora,
           slog->duzinaTrajanjaPregleda);
}

SLOG *pronadjiSlog(FILE *fajl, char *sifraPregleda) {
    if (fajl == NULL) {
        printf("Error: Datoteka nije otvorena!\n");
        return NULL;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while (fread(&blok, sizeof(BLOK), 1, fajl)) {
        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++) {

            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("Dati pregled nije pronadjen!\n");
                return NULL;
            }

            if (strcmp(blok.slogovi[i].sifraPregleda, sifraPregleda) == 0 && !blok.slogovi[i].deleted) {
                SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
                memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                return slog;
            }
        }
    }

    return NULL;
}

void azurirajSlog(FILE *fajl, char *sifraPregleda, char *brojKartona, int duzinaPregleda) {
    if (fajl == NULL) {
        printf("Error: Datoteka nije otvorena!\n");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while (fread(&blok, sizeof(BLOK), 1, fajl)) {
        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++) {

            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("Zeljeni slog nije pronadjen!\n");
                return;
            }

            if (strcmp(blok.slogovi[i].sifraPregleda, sifraPregleda) == 0 &&
                    !blok.slogovi[i].deleted) {
                strcpy(blok.slogovi[i].brojKartona, brojKartona);
                blok.slogovi[i].duzinaTrajanjaPregleda = duzinaPregleda;
                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                return;
            }

        }

    }
}

void logickiObrisiSlog(FILE *fajl, char *sifraPregleda) {
    if (fajl == NULL) {
        printf("Error: Datoteka nije otvorena!\n");
        return;
    }

    BLOK blok;
    fseek(fajl, 0, SEEK_SET);

    while (fread(&blok, sizeof(BLOK), 1, fajl)) {

        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++) {
            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("Ne postoji podatak o tom pregledu.\n");
                return;
            }

            if (strcmp(blok.slogovi[i].sifraPregleda, sifraPregleda) == 0 &&
                    !blok.slogovi[i].deleted) {
                        blok.slogovi[i].deleted = 1;
                        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOK), 1, fajl);
                        printf("Uspesno obrisan podatak!\n");
                        return;
                    }
        }
    }
}

void fizickiObrisiSlog(FILE *fajl, char *sifraPregleda) {
    if (fajl == NULL) {
        printf("Error: Nije otvorena datoteka!\n");
        return;
    }

    BLOK blok, naredniBlok;
    char *sifraZaBrisanje[6];
    strcpy(sifraZaBrisanje, sifraPregleda);
    fseek(fajl, 0, SEEK_SET);

    while (fread(&blok, sizeof(BLOK), 1, fajl)) {

        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++) {

            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {

                if (i == 0) {
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
                    long bytesToKeep = ftell(fajl);
                    ftruncate(fileno(fajl), bytesToKeep);
                    fflush(fajl);
                    return;
                }

                return;
            }

            if (strcmp(blok.slogovi[i].sifraPregleda, sifraZaBrisanje) == 0) {
                if (strcmp(blok.slogovi[i].sifraPregleda, sifraPregleda) == 0 && blok.slogovi[i].deleted)
                    continue;


                for (int j = i + 1; j < FAKTOR_BLOKIRANJA; j++) {
                    memcpy(&blok.slogovi[j-1], &blok.slogovi[j], sizeof(SLOG));
                }

                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);

                if (podatakaProcitano) {

                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    memcpy(&blok.slogovi[FAKTOR_BLOKIRANJA - 1], &naredniBlok.slogovi[0], sizeof(SLOG));
                    strcpy(sifraZaBrisanje, naredniBlok.slogovi[0].sifraPregleda);
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fseek(fajl, 0, SEEK_CUR);

                if (!podatakaProcitano) {
                    printf("Success!");
                    return;
                }
            }
        }
    }

}

void upisiUSekvencijalnu(FILE *fajlSerijski, FILE *fajlSekvencijalni) {
    if (fajlSerijski == NULL || fajlSekvencijalni == NULL) {
        printf("Error.");
        return;
    }

    BLOK blok;
    fseek(fajlSerijski, 0, SEEK_SET);
    fseek(fajlSekvencijalni, 0, SEEK_SET);

    while(fread(&blok, sizeof(BLOK), 1, fajlSerijski)) {

        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++) {

            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                printf("Done.");
                return;
            }

            if (!blok.slogovi[i].deleted) {
                long whereWeLeftOff = ftell(fajlSerijski);
                int no = brojPregledaPacijenta(fajlSerijski, blok.slogovi->brojKartona);
                fseek(fajlSerijski, whereWeLeftOff, SEEK_SET);

                if (no > 1) {
                    SLOGS slog;
                    strcpy(slog.brojKartona, blok.slogovi[i].brojKartona);
                    strcpy(slog.imePacijenta, blok.slogovi[i].imePacijenta);
                    slog.brojPregleda = no;
                    slog.deleted = 0;
                    dodajUSekvencijalnu(fajlSekvencijalni, &slog);

                }
            }
        }
    }

}

void dodajUSekvencijalnu(FILE *fajl, SLOGS *slog) {
    if (fajl == NULL) {
        printf("Error.");
        return;
    }

    SLOGS *slogFound = pronadjiSlogUSekvencijalnoj(fajl, slog->brojKartona);
    if (slogFound != NULL) {
        return;
    }

    BLOKS blok, naredniBlok;
    fseek(fajl, 0, SEEK_SET);
    SLOG slogKojiUpisujemo;
    memcpy(&slogKojiUpisujemo, slog, sizeof(SLOGS));

    while (fread(&blok, sizeof(BLOKS), 1, fajl)) {

        for (int i = 0; i < FAKTOR_BLOKIRANJA2; i++) {

            if (strcmp(blok.slogovi[i].brojKartona, OZNAKA_KRAJA_DATOTEKE) == 0) {
                memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOGS));

                if (i != FAKTOR_BLOKIRANJA2 - 1) {
                    strcpy(blok.slogovi[i+1].brojKartona, OZNAKA_KRAJA_DATOTEKE);
                    fseek(fajl, -sizeof(BLOKS), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOKS), 1, fajl);
                    return;

                } else {
                    fseek(fajl, -sizeof(BLOKS), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOKS), 1, fajl);

                    BLOKS noviBlok;
                    strcpy(noviBlok.slogovi[0].brojKartona, OZNAKA_KRAJA_DATOTEKE);
                    fwrite(&noviBlok, sizeof(BLOKS), 1, fajl);
                    return;
                }
            } else if (strcmp(blok.slogovi[i].brojKartona, slogKojiUpisujemo.brojKartona) == 0) {
                    if (blok.slogovi[i].deleted) {
                        memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOGS));
                        fseek(fajl, -sizeof(SLOGS), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOKS), 1, fajl);
                        return;
                    }
                } else if (atoi(blok.slogovi[i].brojKartona) > atoi(slogKojiUpisujemo.brojKartona)) {
                    SLOGS temp;
                    memcpy(&temp, &blok.slogovi[i], sizeof(SLOGS));
                    memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOGS));
                    memcpy(&slogKojiUpisujemo, &temp, sizeof(SLOGS));

                    if (i == FAKTOR_BLOKIRANJA2 - 1) {
                        fseek(fajl, -sizeof(BLOKS), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOKS), 1, fajl);
                        fseek(fajl, 0, SEEK_CUR);

                    }
                }
            }
        }
}

int brojPregledaPacijenta(FILE *fajl, char *brojKartona) {
    int ret = 0;
    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while (fread(&blok, sizeof(BLOK), 1, fajl)) {

        for (int i = 0; i < FAKTOR_BLOKIRANJA; i++) {

            if (strcmp(blok.slogovi[i].sifraPregleda, OZNAKA_KRAJA_DATOTEKE) == 0) {
                return ret;
            }

            if (strcmp(blok.slogovi[i].brojKartona, brojKartona) == 0 && !blok.slogovi[i].deleted) {
                ret++;
            }
        }
    }
}

SLOGS *pronadjiSlogUSekvencijalnoj(FILE *fajl, char *brojKartona) {
    if (fajl == NULL) {
        printf("Error.");
        return;
    }

    BLOKS blok;
    fseek(fajl, 0, SEEK_SET);

    while (fread(&blok, sizeof(BLOKS), 1, fajl)) {

        for (int i = 0; i < FAKTOR_BLOKIRANJA2; i++) {
            if (strcmp(blok.slogovi[i].brojKartona, OZNAKA_KRAJA_DATOTEKE) == 0
                 || atoi(blok.slogovi[i].brojKartona) > atoi(brojKartona)) {
                printf("Nema datog podatka.");
                return NULL;
            }

            if (strcmp(blok.slogovi[i].brojKartona, brojKartona) == 0 &&
                    !blok.slogovi[i].deleted) {

                        SLOGS *slog = (SLOGS *) malloc(sizeof(SLOGS));
                        memcpy(slog, &blok.slogovi[i], sizeof(SLOGS));
                        return slog;
                     }
        }
    }

    return NULL;
}

void ispisiSveSekv(FILE *fajl) {
    if(fajl == NULL){
        printf("Error");
        return;
    }

    BLOKS blok;
    fseek(fajl, 0, SEEK_SET);

    printf("\nBROJ KARTONA    IME PACIJENTA   BROJ\n");

    while(fread(&blok,sizeof(BLOKS),1,fajl)){
        for(int i=0;i<FAKTOR_BLOKIRANJA2;i++){
            if(strcmp(blok.slogovi[i].brojKartona,OZNAKA_KRAJA_DATOTEKE)==0){
                return;
            }else{
                if(!blok.slogovi[i].deleted){
                    ispisiSlogSekv(&blok.slogovi[i]);
                }
            }
        }
    }
}

void ispisiSlogSekv(SLOGS *slog) {
    printf("       %s           %s      %d\n", slog->brojKartona, slog->imePacijenta,slog->brojPregleda);

}
