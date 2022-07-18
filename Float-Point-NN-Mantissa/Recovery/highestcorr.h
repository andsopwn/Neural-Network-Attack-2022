// highestcorr.h

#ifndef _HEADER_HC_
#define _HEADER_HC_

#include "../MantissaTimingTable/data.h"

#define startpt 0
#define endpt   24000

typedef struct {
    int     maxloc;
    int     maxwt;
    float   maxcorr;
} cr;

double *cutting(const double *cut, const int part) {
    double trace[128];
    for(int i = 0 ; i < 128 ; i++)
        trace[i] = cut[i + (128 * part)];
    return (double *)cut;
}

void init(cr *unname, int num) {
    unname->maxcorr = num;
    unname->maxloc  = num;
    unname->maxwt   = num;
}

#endif