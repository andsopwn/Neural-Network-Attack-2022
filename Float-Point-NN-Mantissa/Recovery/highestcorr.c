#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "highestcorr.h"

double corr(float *x, int *y, int size)
{
    double      Sy      = 0;
    double      Syy     = 0;
    double      Sx      = 0;
    double      Sxx     = 0;
    double      Sxy     = 0;

    // var(X) = E(X^2) - E(X)^2
    for(int i = 0 ; i < size ; i++) {
        Sxy += x[i] * y[i];
        Sx  += x[i];
        Sy  += y[i];
        Sxx += x[i] * x[i];
        Syy += y[i] * y[i];
    }
    return ((double)size*Sxy - Sx * Sy) / sqrt(((double)size * Sxx - Sx * Sx)*((double)size * Syy - Sy* Sy));
}

void highest() {
    FILE    *FN;
    float   **data;
    float   **cut;
    int     trNum       = 5;
    int     trLen       = 24000;
    int     windowsize  = 128;
    int     stepsize    = 1;
    int     repeat      = (int)((trLen - 128)/stepsize);

    int     local_maxloc;
    int     local_maxwt;
    float   local_maxcorr;
    float   local_curcorr;

    int     global_maxloc;
    int     global_maxwt;
    float   global_maxcorr;


    int     i, j, k;

    // File Stream
    FN = fopen("FNN162_625.bin", "rb");
    if(FN == NULL)     puts("TRC OPEN ERR");

    data = (float**)calloc(sizeof(float*), trNum);
    for(i = 0 ; i < trNum ; i++)   
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, FN);
    fclose(FN);

    cut = (float**)calloc(sizeof(float*), repeat);
    for(i = 0 ; i < repeat ; i++)
        cut[i] = (float*)calloc(sizeof(float), windowsize);

    for(i = 0 ; i < repeat ; i++) {
        for(j = 0 ; j < windowsize ; j++)
            cut[i][j] = data[0][stepsize * i + j];
    }

    global_maxcorr = 0;
    global_maxloc = 0;
    global_maxwt = 0;
    for(i = 0 ; i < repeat ; i++) {
        local_maxcorr = 0;
        local_maxloc  = 0;
        local_maxloc  = 0;
        local_maxwt   = 0;
        for(j = 0 ; j < 128 ; j++) {
            
            local_curcorr = fabs(corr(cut[i], timing[j], windowsize));
            
            if(local_curcorr > local_maxcorr) {
                local_maxcorr  = local_curcorr;
                local_maxloc   = (i * stepsize);
                local_maxwt    = j;
            }
        }
        
        if(local_maxcorr > global_maxcorr) {
            global_maxcorr = local_maxcorr;
            global_maxloc  = local_maxloc;
            global_maxwt   = local_maxwt;
        }
        //printf("C[%f] Loc[%d-%d] WT[%f]\n", local_maxcorr, local_maxloc, local_maxloc + 128, (float)local_maxwt / 128 + 1);
    }
    printf("C[%f] Loc[%d-%d] WT[%f]\n", global_maxcorr, global_maxloc, global_maxloc + 128, (float)global_maxwt / 128 + 1);

    for(i = 0 ; i < trNum ; i++)   free(data[i]);
    free(data);
    for(i = 0 ; i < repeat ; i++)   free(cut[i]);
    free(cut);
}

int main() {
    highest();

    
}