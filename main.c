#include "structs.h"



int main(int argc, char const *argv[]) {
    char tip[4] = {0}, chr = 0;
    int width = 0, height = 0, max_clr_val = 0;
    TCuloare **mat_clr = NULL;
    if (argc == long_param) {
        int factor = atoi(argv[2]);
        FILE *fin = fopen(argv[3], "rb");
        if (!fin)
            return -1;
        fscanf(fin, "%s", tip);
        fscanf(fin, "%d", &width);
        fscanf(fin, "%d", &height);
        fscanf(fin, "%d", &max_clr_val);
        fscanf(fin, "%c", &chr);
        // Copiere imagine
        mat_clr = (TCuloare**)malloc(height * sizeof(TCuloare*));
        if (!mat_clr)
            return -1;
        for (int i = 0; i < height; i++) {
            mat_clr[i] = (TCuloare*)malloc(width * sizeof(TCuloare));
            if (!mat_clr[i]) {
                for (int j = 0; j < i; j++)
                    free(mat_clr[j]);
                free(mat_clr);
                return -1;
            }
            fread(mat_clr[i], sizeof(TCuloare), width, fin);
        }
        // Constructie arbore
        int maxi = 0;
        TArb arb = ConstrArbC(mat_clr, height, factor, 0, 0, &maxi);
        // afisare cerinta 1
        if (strstr(argv[1], "c1")) {
            // Cerinta 1
            FILE *fout = fopen(argv[4], "wt");
            if (!fout)
                return -1;
            fprintf(fout, "%d\n%d\n%d\n", NrNiv(arb), NrBloc(arb), maxi);
            fclose(fout);
        } else {
            // Cerinta 2
            FILE *fout = fopen(argv[4], "wb");
            if (!fout)
                return -1;
            PrintBin(arb, width, fout);
            fclose(fout);
        }
        DistrArb(&arb);
        // Distrugere matrice
        for (int i = 0; i < width; i++) {
            free(mat_clr[i]);
            mat_clr[i] = NULL;
        }  
        free(mat_clr);
        mat_clr = NULL;
        fclose(fin);
    } else {
        // Cerinta 3
        FILE *fin = fopen(argv[2], "rb");
        if (!fin)
            return -1;
        unsigned int size = 0;
        TArb arb = DecIm(fin, &size);
        TCuloare **mat_clr = AlocaMat(size);
        ConstrMat(mat_clr, arb, size, 0, 0);
        FILE *fout = fopen(argv[3], "wb");
        fprintf(fout,"P6\n");
        fprintf(fout, "%d %d\n", size, size);
        fprintf(fout, "255\n");
        for (int i = 0; i < (int)size; i++)
            fwrite(mat_clr[i], sizeof(TCuloare), size, fout);
        // Distrugere arbore
        DistrArb(&arb);
        // Distrugere matrice
        for (int i = 0; i < (int)size; i++) {
            free(mat_clr[i]);
            mat_clr[i] = NULL;
        }  
        free(mat_clr);
        mat_clr = NULL;
        fclose(fin);
        fclose(fout);
    }
    return 0;
}