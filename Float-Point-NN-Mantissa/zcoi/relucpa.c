#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    int     maxloc;
    int     maxwt;
    float   maxcorr;
    //long double sum;
} cr;

void init(cr *unname, int num) {
    unname->maxcorr = num;
    unname->maxloc  = num;
    unname->maxwt   = num;
    //unname->sum     = 0;
}

double correlation(const float *x, const int *y, int size) {
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

unsigned int   int32LE(unsigned char val[4]) {
    return val[0] &0xff | (val[1] << 8) & 0x0000ff00 | (val[2] << 16) & 0x00ff0000 | (val[3] << 24) & 0xff000000;
}

unsigned int float_to_int32(float f) {
    union conv {
        float f;
        unsigned int a;
    };
    union conv ff;
    ff.f = f;
    //printf("%08x\n", ff.a);
    return ff.a;
}

float   weight[5] = { 0.25975, -0.208775, -0.0169609375, 0.295075, 0.00879296875 };

cr  inCPA(int bitloc) {
    cr      local, global;
    FILE    *RFP, *YFP, *WFP;
    float   *corr;
    float   *cutX;
    int     *cutY;
    float   **data;
    int     trNum       = 255;
    int     trLen       = 20000;
    int     wt, loc;
    float         on;
    unsigned int  in;

    char    FN[100];    sprintf(FN, "../Trace/%s.bin", "mlp"); // trace
    char    CN[100];    sprintf(CN, "../Trace/%s.crr", "rel"); // corr

    if((RFP = fopen(FN, "rt")) == NULL) puts("MAIN :: TRACE FILE IS NOT DETECTED");

    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP);
    
    cutX = (float*)calloc(sizeof(float), trNum);
    cutY = (int*)calloc(sizeof(int), trNum);
    corr = (float*)calloc(sizeof(double), trLen);

    for(int i = 0 ; i < trNum ; i++) {
        on = (float)i * weight[0];
        in = float_to_int32(on);
        for(int k = 0 ; k < 32 ; k++) cutY[i] += (in >> k) & 0b1;
    }

    for(loc = 0 ; loc < trLen ; loc++) {
        for(int i = 0 ; i < trNum ; i++)
            cutX[i] = data[i][loc];

        corr[loc] = correlation(cutX, cutY, trNum);
        if(fabs(corr[loc]) > local.maxcorr) {
            local.maxcorr = fabs(corr[loc]);
            local.maxloc  = loc;
            local.maxwt   = wt;
        }
    }
    if(bitloc == 0)        WFP = fopen(CN, "w+b");
    else                   WFP = fopen(CN, "a+b");

    fwrite(corr, sizeof(float), trLen, WFP);
    fclose(WFP);
    //printf("[%lf], [%d]\n", local.maxcorr, local.maxloc);

    for(int i = 0 ; i < trNum ; i++)
        free(data[i]);
    free(data);
    free(corr);
    free(cutX);
    free(cutY);
 
    return local;
}

int main() {
    puts("\t:: MMTB Experiment ::\t");
    cr    in[5];

    for(int i = 0 ; i < 5 ; i++) { in[i] = inCPA(i); printf("INPUT LOCATION(%d)\t[%d] (%.2f%%)\n", i, in[i].maxloc, in[i].maxcorr * 100); }
}