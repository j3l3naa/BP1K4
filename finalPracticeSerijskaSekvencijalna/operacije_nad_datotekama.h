#ifndef OPERACIJE_NAD_DATOTEKAMA_H_INCLUDED
#define OPERACIJE_NAD_DATOTEKAMA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "definicija_strukutre_podataka.h"

void kreirajDatoteku(char *filename);
FILE *otvoriDatoteku(char *filename);
void kreirajSekvencijalnuDatoteku(char *filename);
void dodajSlogUSerijsku(FILE *fajl, SLOG *slog);
void ispisiSlogove(FILE *fajl);
void ispisiSlog(SLOG *slog);
SLOG *pronadjiSlog(FILE *file, char *sifraPregleda);
void azurirajSlog(FILE *fajl, char *sifraPregleda, char *brojKartona, int duzinaPregleda);
void logickiObrisiSlog(FILE *fajl, char *sifraPregleda);
void fizickiObrisiSlog(FILE *fajl, char *sifraPregleda);
void upisiUSekvencijalnu(FILE *fajlSerijski, FILE *fajlSekvencijalni);
void dodajUSekvencijalnu(FILE *fajl, SLOGS *slog);
int brojPregledaPacijenta(FILE *fajl, char *brojKartona);
SLOGS *pronadjiSlogUSekvencijalnoj(FILE *fajl, char *brojKartona);
void ispisiSveSekv(FILE *fajl);
void ispisiSlogSekv(SLOGS *slog);

#endif // OPERACIJE_NAD_DATOTEKAMA_H_INCLUDED
