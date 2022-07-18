#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "highestcorr.h"

double correlation(const float *x, const float *y) {
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
    float   cutX[128], cutY[128];
    int     trNum       = 5;
    int     trLen       = 24000;
    int     windowsize  = 128;
    int     stepsize    = 1;

    float   curcorr;
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

    /* 실험 실패 이유
         cut data 는 128 단위이므로 3차 배열 또는 2차 배열을 여러번 바꿔주면서 실험해야함 하지만 지금은 그렇지 않기에 결과값이 나오지 않음 
        코드 뜯어 고쳐라
    */
    
    cr  local, global;

    init(&global, 0);
    for(i = 0 ; i < 0x7f ; i++) {
        init(&local, 0);    
        for(j = 0 ; j < (trLen - windowsize) / stepsize ; j++) {
        init(&local, 0);    
            for(k = 0 ; k < 128 ; k++) {
                cutX[k] = data[1][k + j * stepsize];
                cutY[k] = timing[i][k];
            }
            curcorr = correlation(cutX, (float *)cutY);
            if(curcorr >= 1.0 || curcorr <= -1.0)  { puts("FAULT!"); return; }
            
            if(curcorr > local.maxcorr) {
                local.maxcorr = curcorr;
                local.maxwt   = i;
                local.maxloc  = j;
            }
        }
        if(local.maxcorr > global.maxcorr) {
            global.maxcorr = local.maxcorr;
            global.maxloc  = local.maxloc;
            global.maxwt   = local.maxwt;
        }
        //printf("%lf\n", curcorr);

    }
    printf("C[%f] Loc[%d-%d] WT[%f](%d)\n", global.maxcorr, global.maxloc, global.maxloc + 128, (float)global.maxwt / 128 + 1, global.maxwt);
    for(i = 0 ; i < trNum ; i++)   free(data[i]);
    free(data);
}

int main() {
    highest();
    
}