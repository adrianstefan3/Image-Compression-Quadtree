#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nr_nod_ac 4
#define long_param 5
#define img_max_size 4096

// Aceasta structura declara un pixel
// format din culorile rosu, verde si albasru
typedef struct color {
    unsigned char R, G, B;
} TCuloare;

// Aceasta structura declara arborele cuaternar
// ss = sus-stanga; sd = sus-dreapta
// jd = jos-dreapta; js = jos-stanga
typedef struct nod {
    TCuloare culoare;
    unsigned char tip;
    struct nod *ss, *sd, *jd, *js;
} TNod, *TArb;

// Aceasta structura declara o celula a cozii in
// care informatia este adresa catre un nod din arbore
typedef struct celula {
    TArb info;
    struct celula *urm;
} TCelula, *TLista;

// Aceasta structura reprezinta coada de noduri ale arborelui
typedef struct coada {
    TLista inc, sf;
} TCoada;

// Aceasta structura reprezinta un vector de noduri din arbore
typedef struct node {
    unsigned char tip;
    TCuloare color;
} THeapQT;

TArb AlocaNod(TCuloare clr, unsigned char tip);
int CalcScorSim(TCuloare **mat_clr, int size, int x, int y, int factor, TCuloare *clr);
TArb ConstrArbC(TCuloare **mat_clr, int size, int factor, int x, int y, int *maxi);
int max(int a, int b);
int min(int a, int b);
int NrNiv(TArb arb);
int NrBloc(TArb arb);
void distruge(TArb r);
void DistrArb(TArb *a);
TLista AlocaCelula(TArb x);
TCoada* InitQ();
int InsrtQ(TCoada *c, TArb x);
int ExtrQ(TCoada *c);
void PrintBin(TArb arb, int size, FILE *fout);
void DistrugeQ(TCoada **c);
TArb DecIm(FILE *fin, unsigned int *size);
void Coloreaza(TCuloare **mat_clr, TCuloare clr, int size, int x, int y);
void ConstrMat(TCuloare **mat_clr, TArb arb, int size, int x, int y);
TCuloare** AlocaMat(int size);