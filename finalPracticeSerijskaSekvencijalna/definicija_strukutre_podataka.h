#ifndef DEFINICIJA_STRUKUTRE_PODATAKA_H_INCLUDED
#define DEFINICIJA_STRUKUTRE_PODATAKA_H_INCLUDED

#define FAKTOR_BLOKIRANJA 4
#define OZNAKA_KRAJA_DATOTEKE "******"
#define FAKTOR_BLOKIRANJA2 3

typedef struct SlogSerijske {
    char brojKartona[5+1];
    int duzinaTrajanjaPregleda;
    char imeDoktora[20];
    char sifraPregleda[6];
    char imePacijenta[20];
    int deleted;
} SLOG;

typedef struct Blok {
    SLOG slogovi[FAKTOR_BLOKIRANJA];
} BLOK;

typedef struct SlogSekvencijalne {
    char imePacijenta[20];
    char brojKartona[6];
    int brojPregleda;
    int deleted;
} SLOGS;

typedef struct BlokS {
    SLOGS slogovi[FAKTOR_BLOKIRANJA2];
} BLOKS;

#endif // DEFINICIJA_STRUKUTRE_PODATAKA_H_INCLUDED
