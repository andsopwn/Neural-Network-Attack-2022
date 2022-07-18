#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "highestcorr.h"

double correlation(float *x, int *y) {
    int         size    = 128;
    double      Sy      = 0;
    double      Syy     = 0;
    double      Sx      = 0;
    double      Sxx     = 0;
    double      Sxy     = 0;

    // var(X) = E(X^2) - E(X)^2
    for(int i = 0 ; i < size ; i++) {
        Sxy += x[i] * (float)y[i];
        Sx  += x[i];
        Sy  += (float)y[i];
        Sxx += x[i] * x[i];
        Syy += (float)y[i] * (float)y[i];
    }
    return ((double)size*Sxy - Sx * Sy) / sqrt(((double)size * Sxx - Sx * Sx)*((double)size * Syy - Sy* Sy));
}

void highest() {
    FILE    *FN, *WF;
    float   **data;
    float   **cut;
    float   temp[128];
    double  corr;
    int     trNum       = 5;
    int     trLen       = 24000;
    int     i, j, k;
    char    buf[256];

    // File Stream
    FN = fopen("f.bin", "rb");
    if(FN == NULL)     puts("TRC OPEN ERR");

    data = (float**)calloc(sizeof(float*), trNum);
    for(i = 0 ; i < trNum ; i++)   
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, FN);
    fclose(FN);

    for(i = 0 ; i < 128 ; i++) {
        temp[i] = data[0][i + (128) * 10];
    }

    

    corr = correlation(temp, (int *)timing[64]);

    printf("%f\n", corr);
    


    for(i = 0 ; i < trNum ; i++)   free(data[i]);
    free(data);
}

int main() {
    highest();
    
}