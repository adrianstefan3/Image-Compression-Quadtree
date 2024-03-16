#include "structs.h"

// Aceasta functie aloca un nod al arborelui si ii adauga
// ca informatie culoarea si tipul nodului
TArb AlocaNod(TCuloare clr, unsigned char tip) {
    TArb aux = (TArb)malloc(sizeof(TNod));
    if (!aux)
        return NULL;
    aux->culoare = clr;
    aux->tip = tip;
    aux->jd = aux->js = aux->sd = aux->ss = NULL;
    return aux;
}

// Aceasta functie calculeaza scorul similaritatii
// si returneaza 1 daca acesta este <= factorul, iar 0
// in caz contrar
int CalcScorSim(TCuloare **mat_clr, int size, int x, int y, int factor, TCuloare *clr) {
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            red += mat_clr[i][j].R;
            green += mat_clr[i][j].G;
            blue += mat_clr[i][j].B;
        }
    }
    red /= (size * size);
    green /= (size * size);
    blue /= (size * size);
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            mean += (red - mat_clr[i][j].R) * (red - mat_clr[i][j].R);
            mean += (green - mat_clr[i][j].G) * (green - mat_clr[i][j].G);
            mean += (blue - mat_clr[i][j].B) * (blue - mat_clr[i][j].B);
        }
    }
    mean = mean / (3 * (size * size));
    if ((int)mean <= factor) {
        (*clr).R = red;
        (*clr).G = green;
        (*clr).B = blue;
        return 1;
    }
    return 0;
}

// Aceasta functie imparte matricea in 4 submatrice si tot
// asa pana cand scorul este <= factorul, caz in care retine in nodul
// respectiv culorile calculate in functia de mai sus
TArb ConstrArbC(TCuloare **mat_clr, int size, int factor, int x, int y, int *maxi) {
    TCuloare color = {0, 0, 0};
    int ok = CalcScorSim(mat_clr, size , x, y, factor, &color);
    TArb aux = AlocaNod(color, (unsigned char)ok);
    if (!aux)
        return NULL;
    if (ok == 1) {
        if (*maxi < size)
            *maxi = size;
        return aux;
    }
    // Construire sub-arbore sus-stanga
    aux->ss = ConstrArbC(mat_clr, size / 2, factor, x, y, maxi);
    // Construire sub-arbore sus-dreapta
    aux->sd = ConstrArbC(mat_clr, size / 2, factor, x, y + (size / 2), maxi);
    // Construire sub-arbore jos-dreapta
    aux->jd = ConstrArbC(mat_clr, size / 2, factor, x + (size / 2), y + (size / 2), maxi);
    // Construire sub-arbore jos-stanga
    aux->js = ConstrArbC(mat_clr, size / 2, factor, x + (size / 2), y, maxi);
    return aux;
}

// Returneaza maximul dintre doua numere
int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

// Aceasta functie returneaza numarul de niveluri ale arborelui
int NrNiv(TArb arb) {
    int nss = 0, nsd = 0, njs = 0, njd = 0;
    if (!arb)
        return 0;
    nss = NrNiv(arb->ss);
    nsd = NrNiv(arb->sd);
    njs = NrNiv(arb->js);
    njd = NrNiv(arb->jd);
    return 1 + max(max(nss, nsd), max(njs, njd));
}

// Aceasta functie returneaza numarul de blocuri din imagine al caror
// scor este <= factorul (numarul de frunze)
int NrBloc(TArb arb) {
    int n = 0;
    if(!arb) return 0;
    if (arb->tip == 1)
        n++;
    n += NrBloc(arb->ss);
    n += NrBloc(arb->sd);
    n += NrBloc(arb->js);
    n += NrBloc(arb->jd);
    return n;
}

// Aceasta functie este o functie auxiliara pentru distrugerea arborelui
// distrugand cei patru fii ai unui nod
void distruge(TArb r) {
	if (!r) return;
	distruge(r->ss);
	distruge(r->sd);
    distruge(r->js);
    distruge(r->jd);
	free(r);
    r = NULL;
}

// Aceasta functie distruge arborele cuaternar
void DistrArb(TArb *a) {
	if (!(*a)) return;
	distruge (*a);
	*a = NULL;
}

// Aceasta functie aloca o celula pentru coada
TLista AlocaCelula(TArb x) {
    TLista aux = (TLista)malloc(sizeof(TCelula));
    if (!aux)
        return NULL;
    aux->info = x;
    aux->urm = NULL;
    return aux;
}

// Aceasta functie initializeaza o coada vida
TCoada* InitQ() {
    TCoada *c = NULL;
    c = (TCoada*)malloc(sizeof(TCoada));
    if (!c)
        return NULL;
    c->inc = c->sf = NULL;
    return c;
}

// Aceasta functie introduce un element de tip arbore in coada
int InsrtQ(TCoada *c, TArb x) {
    TLista aux = AlocaCelula(x);
    if (!aux)
        return 0;
    if (c->inc == NULL)
        c->inc = c->sf = aux;
    else {
        c->sf->urm = aux;
        c->sf = aux;
    }
    return 1;
}

// Aceasta functie sterge primul element din coada fara al memora
int ExtrQ(TCoada *c) {
    if (c->inc == NULL)
        return 0;
    TLista aux = c->inc;
    c->inc = aux->urm;
    aux->info = NULL;
    free(aux);
    if (c->inc == NULL)
        c->sf = NULL;
    return 1;
}

// Aceasta functie scrie in fisierul binar primit ca paramentru
// parcurgerea in latime a arborelui cuaternar 'arb'
// dupa modelul: tip (0 sau 1) si R, G, B (doar in cazul in care tip = 1);
// Pentru parcurgerea BFS am retinut pe rand parintele si fii sai in coada
// dupa care am scos la fiecare pas cate un nod pana coada a ramas vida
void PrintBin(TArb arb, int size, FILE *fout) {
    if (arb == NULL)
        return;
    TCoada *c = InitQ();
    if(!InsrtQ(c, arb))
        return;
    fwrite(&size, sizeof(unsigned int), 1, fout);
    while (c->inc != NULL) {
        fwrite(&(c->inc->info->tip), sizeof(unsigned char), 1, fout);
        arb = c->inc->info;   
        if (c->inc->info->tip == 1) {
            fwrite(&(c->inc->info->culoare.R), sizeof(unsigned char), 1, fout);
            fwrite(&(c->inc->info->culoare.G), sizeof(unsigned char), 1, fout);
            fwrite(&(c->inc->info->culoare.B), sizeof(unsigned char), 1, fout);
        } else {
            if(!InsrtQ(c, arb->ss))
                return;
            if(!InsrtQ(c, arb->sd))
                return;
            if(!InsrtQ(c, arb->jd))
                return;
            if(!InsrtQ(c, arb->js))
                return;
        }
        if (!ExtrQ(c))
            return;
    }
    DistrugeQ(&c);
}

// Aceasta functie distruge o coada vida
void DistrugeQ(TCoada **c) {
    (*c)->inc = (*c)->sf = NULL;
    free(*c);
    *c = NULL;
}

// Aceasta functie spre deosebire de ExtrQ, extrage primul element
// din coada, retinandu-l
int ExtrVal(TCoada *c, TArb *ax) {
    if (c->inc == NULL)
        return 0;
    *ax = c->inc->info;
    TLista aux = c->inc;
    c->inc = aux->urm;
    aux->info = NULL;
    free(aux);
    if (c->inc == NULL)
        c->sf = NULL;
    return 1;
}

// Aceasta functie citeste din fisierul binar primit
// ca argument si construieste arborele cuaternar
// corespunzator imaginii folosind un vector de noduri
// si o coada, in care introduce pe rand parintii
// pe care ii extrage apoi si ii leaga la copii
TArb DecIm(FILE *fin, unsigned int *size_img) {
    unsigned int size = 0, cnt = 0, poz = 0;
    fread(&size, sizeof(unsigned int), 1, fin);
    // qtree reprezinta un vector de noduri ale unui arbore
    // cuaternar, unde fii nodului i ii determin prin formula
    // i * 4 + k, k = {1, 2 , 3, 4}
    THeapQT *qtree = (THeapQT*)malloc(img_max_size * img_max_size * sizeof(THeapQT));
    // img_max_size este marimea maxima pe care o poate avea o imagine
    if (!qtree)
        return NULL;
    unsigned char tip = 0;
    while (fread(&tip, sizeof(unsigned char), 1, fin)) {
        qtree[cnt].tip = tip;
        if (tip == 1)
            fread(&(qtree[cnt].color), sizeof(TCuloare), 1, fin);
        cnt++;
    }
    TArb arb = AlocaNod(qtree[0].color,qtree[0].tip), aux = NULL;
    aux = arb;
    TCoada *c = InitQ();
    if (!c)
        return NULL;
    if (arb->tip == 0) {
        if (!InsrtQ(c, arb))
            return NULL;
        while (c->inc != NULL) {
            if (!ExtrVal(c, &aux))
                return NULL;
            aux->ss = AlocaNod(qtree[poz * 4 + 1].color, qtree[poz * 4 + 1]. tip);
            aux->sd = AlocaNod(qtree[poz * 4 + 2].color, qtree[poz * 4 + 2]. tip);
            aux->jd = AlocaNod(qtree[poz * 4 + 3].color, qtree[poz * 4 + 3]. tip);
            aux->js = AlocaNod(qtree[poz * 4 + 4].color, qtree[poz * 4 + 4]. tip);
            if (aux->ss->tip == 0)
                if (!InsrtQ(c, aux->ss))
                    return NULL;
            if (aux->sd->tip == 0)
                if (!InsrtQ(c, aux->sd))
                    return NULL;
            if (aux->jd->tip == 0)
                if (!InsrtQ(c, aux->jd))
                    return NULL;
            if (aux->js->tip == 0)
                if (!InsrtQ(c, aux->js))
                    return NULL;
            poz++;
        }
    }
    // Distrugere coada si vector de arbori
    free(qtree);
    qtree = NULL;
    DistrugeQ(&c);
    *size_img = size;
    return arb;
}

// Aceasta functie coloreaza un bloc de pixeli
// care nu a mai fost divizat
void Coloreaza(TCuloare **mat_clr, TCuloare clr, int size, int x, int y) {
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            mat_clr[i][j].R = clr.R;
            mat_clr[i][j].G = clr.G;
            mat_clr[i][j].B = clr.B;
        }
    } 
}

// Aceasta functie construieste matricea imaginii pe baza
// arborelui cuaternar, astfel merge pe fiecare dintre cele 4
// submatrice si pe diviziunile lor cat timp nodul din arbore
// nu este unul terminal
void ConstrMat(TCuloare **mat_clr, TArb arb, int size, int x, int y) {
    if (arb->tip == 1) {
        Coloreaza(mat_clr, arb->culoare, size, x, y);
    } else {
        ConstrMat(mat_clr, arb->ss, size / 2, x, y);
        ConstrMat(mat_clr, arb->sd, size / 2, x, y + (size / 2));
        ConstrMat(mat_clr, arb->jd, size / 2, x + (size / 2), y + (size / 2));
        ConstrMat(mat_clr, arb->js, size / 2, x + (size / 2), y);
    }
}

// Aceasta Functie aloca o matrice de marimea 'size'
TCuloare** AlocaMat(int size) {
    TCuloare** mat = (TCuloare**)malloc(size * sizeof(TCuloare*));
    if (!mat)
        return NULL;
    for (int i = 0; i < size; i++) {
        mat[i] = (TCuloare*)malloc(size * sizeof(TCuloare));
        if (!mat[i]) {
            for (int j = 0; j < i; j++) {
                free(mat[j]);
                mat[j] = NULL;
            }
            free(mat);
                return NULL;
        }
    }
    return mat;
}