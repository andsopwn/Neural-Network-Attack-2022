#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double correlation(const float *x, const float *y, int size) {
    double      Sy      = 0;
    double      Syy     = 0;
    double      Sx      = 0;
    double      Sxx     = 0;
    double      Sxy     = 0;

    for(int i = 0 ; i < size ; i++) {
        Sxy += (float)x[i] * y[i];
        Sx  += (float)x[i];
        Sy  += (float)y[i];
        Sxx += (float)x[i] * (float)x[i];
        Syy += (float)y[i] * (float)y[i];
    }
    return ((double)size*Sxy - Sx * Sy) / sqrt(((double)size * Sxx - Sx * Sx)*((double)size * Syy - Sy* Sy));
}

int main() {
    FILE    *RFP, *WFP;
    float   **data;
    float   **reel;
    float   *cutX, *cutY;
    float   corr        = 0;
    float   maxcorr     = 0;
    int     maxloc      = 0;
    int     cutsize     = 300;
    int     trLen       = 20000;
    int     trNum       = 255;
    int     rlNum       = 2;
    int     loc[200]    = { 0, };

    RFP = fopen("../Trace/mlp.bin", "rb");
    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP);

    RFP = fopen("../Trace/relu.bin", "rb");
    reel = (float**)calloc(sizeof(float*), rlNum);
    for(int i = 0 ; i < rlNum ; i++)
        reel[i] = (float*)calloc(sizeof(float), trLen);
    for(int i = 0 ; i < rlNum ; i++)
        fread(reel[i], sizeof(float), trLen, RFP);
    fclose(RFP);

    cutX = (float*)calloc(sizeof(float), cutsize); 
    cutY = (float*)calloc(sizeof(float), cutsize); 

    WFP = fopen("report.ee", "w+t");
    //for(int a = 0 ; a < trNum ; a++) {
    for(int i = 0 ; i < 15500 ; i+=1) {
        for(int j = 0 ; j < cutsize ; j++) {
            cutX[j] = reel[1][j + 100];
            cutY[j] = data[0][j + i];
        }

        corr = fabs(correlation(cutX, cutY, cutsize));
        //printf("%f\n", corr);
        if(fabs(corr) > maxcorr) {
            maxcorr = fabs(corr);
            maxloc  = i;
        }
        fprintf(WFP, "%f %d\n", corr, i);
    }
    fflush(stdout);
    //}
    free(cutX);
    free(cutY);
    for(int i = 0 ; i < trNum ; i++)
        free(data[i]);
    free(data);
    for(int i = 0 ; i < rlNum ; i++)
        free(reel[i]);
    free(reel);
}