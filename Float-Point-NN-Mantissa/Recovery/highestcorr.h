// highestcorr.h

#ifndef _HEADER_HC_
#define _HEADER_HC_
#define debug 1
#define startpt 10000
#define endpt   15000

#include "../MantissaTimingTable/data.h"

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

double correlation(const float *x, const float *y) {
    int         size    = 128;

    double      Sy      = 0;
    double      Syy     = 0;
    double      Sx      = 0;
    double      Sxx     = 0;
    double      Sxy     = 0;

    for(int i = 0 ; i < size ; i++) {
        Sxy += (float)x[i] * (float)y[i];
        Sx  += (float)x[i];
        Sy  += (float)y[i];
        Sxx += (float)x[i] * (float)x[i];
        Syy += (float)y[i] * (float)y[i];
    }
    return ((double)size*Sxy - Sx * Sy) / sqrt(((double)size * Sxx - Sx * Sx)*((double)size * Syy - Sy* Sy));
}


#endif