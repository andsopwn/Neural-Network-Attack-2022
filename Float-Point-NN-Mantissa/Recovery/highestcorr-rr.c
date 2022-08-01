#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "highestcorr.h"

float   realweight[5] = { 1+(float)124/128, 1+(float)57/128, 1+(float)84/128, 1+(float)47/128, 1+(float)50/128 };

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

typedef struct {
    int     maxloc;
    int     maxwt;
    float   maxcorr;
    double  highest;
    //long double sum;
} cr;

void init(cr *unname, int num) {
    unname->maxcorr = num;
    unname->maxloc  = num;
    unname->maxwt   = num;
    unname->highest = num;
    //unname->sum     = 0;
}

#define     TIMINGFN    "../Trace/time_all.bin"
#define     TRACEFN     "../Trace/152.bin"
#define     CORRFN      "../Trace/HIGHEST.crr"

void highestCorr() {
    double  corrsum;
    float   weight;
    float   maxcr;
    float   *corr;
    float   **tim;
    float   **mlp;
    float   *cutX;  // mlp trace
    float   *cutY;  // timing trace
    
    unsigned int    trNum       = 5000;
    unsigned int    trLen       = 18000;
    unsigned int    tiNum       = 16384;
    unsigned int    tiLen       = 2000;
    unsigned int    cutsize     = 200;

    cr      local, global;
    FILE    *RFP, *WFP;

    if((RFP = fopen(TRACEFN, "rb")) == NULL)    { puts("STREAM ERR :: TRACEFN"); return; }
    mlp  = (float**)calloc(sizeof(float*), trNum);     
    for(int i = 0 ; i < trNum ; i++) {      
        mlp[i] = (float*)calloc(sizeof(float), trLen);      
        fread(mlp[i], sizeof(float), trLen, RFP);
    }
    fclose(RFP);

    if((RFP = fopen(TIMINGFN, "rb")) == NULL)    { puts("STREAM ERR :: TIMINGFN"); return; }
    tim  = (float**)calloc(sizeof(float*), tiNum);
    for(int i = 0 ; i < tiNum ; i++) {
        tim[i] = (float*)calloc(sizeof(float), tiLen);
        fread(tim[i], sizeof(float), tiLen, RFP);
    }

    cutX = (float*)calloc(sizeof(float), cutsize);
    cutY = (float*)calloc(sizeof(float), cutsize);
    corr = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < cutsize ; i++)
        //cutX[i] = mlp[30][866 + i];
        cutX[i] = mlp[11][138 + i];
    puts("\n");
    init(&global, 0);
    for(int wt = 0 ; wt < 128 ; wt++) {
        
        init(&local, 0);
        for(int loc = 300 ; loc < 600 ; loc+=1) {  // 250 ~ 970

            for(int ip = 0 ; ip < 128 ; ip++) {
                for(int i = 0 ; i < cutsize ; i++)
                    cutY[i] = tim[wt * 128 + ip][i + loc];

                corr[ip] = correlation(cutX, cutY, cutsize);
                //printf("%lf\n", corr[ip]);
                
                if(fabs(corr[ip]) > local.maxcorr) {
                    local.maxcorr = fabs(corr[ip]);
                    local.maxloc  = ip;
                    local.maxwt   = wt;
                }
                //fflush(stdout);
                //printf(" current \t Weight[%d|%d] corr[%lf|%lf] -> timing loc[%d] wt[%d]\r", global.maxwt, local.maxwt, global.maxcorr, local.maxcorr, loc, wt);
            }
            printf("Processing %.2f%%\t\r", (float)wt/128 * 100);
        }
        if(local.maxcorr > global.maxcorr) {
            global = local;
            fflush(stdout);
            printf("\t\t\tMAX VALUE : \t wt[%d] corr[%lf]\r", global.maxwt, global.maxcorr);
        }

        if(wt == 0)     WFP = fopen(CORRFN, "w+b");
        else            WFP = fopen(CORRFN, "a+b");
        fwrite(corr, sizeof(float), 128, WFP);
        fclose(WFP);
    }
    puts("");

    printf("\n :: RECOVERED MANTISSA :: \nPEAK %lf\t%lf (%d)\n", global.maxcorr, 1+(float)global.maxwt/128, global.maxwt);
    printf("\n :: REAL Weight Mantissa ::\n");
    for(int i = 0 ; i < 5 ; i++) {
        printf("Node(%d) \t %lf (%d)\n", i, realweight[i], (int)((realweight[i]-1) * 128));
    }
    puts("");


    free(cutX);
    free(cutY);
    free(corr);

    for(int i = 0 ; i < trNum ; i++)
        free(mlp[i]);
    free(mlp);
    for(int i = 0 ; i < tiNum ; i++)
        free(tim[i]);
    free(tim);
}

int main() {
    highestCorr();
}