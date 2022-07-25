// highestcorr.h

#ifndef _HEADER_HC_
#define _HEADER_HC_
#define debug 1
#define startpt 10000
#define endpt   15000
#define FNinp "corr/inp.bin"
#define FNmul "corr/mul.bin"
#define FNmov "corr/mov.bin"
#define FNexp "corr/exp.bin"

#include "../MantissaTimingTable/data.h"
#include "float32.h"
#include "input32.h"

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

unsigned int   int32LE(unsigned char val[4]) {
    return val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24);
}

cr  inMUL(int bitloc, int windowsize);
cr  inCPA(int bitloc);

#endif