#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "NNCPA.h"

ff  ip;
ff  wt;
ff  tt;

#define     bit      3
#define     DIR     "..//..//..//부채널연구//SCA_Experiment//a.npy"
#define     IPN     1000
#define     STR     -2.55
#define     ENR     2.55
#define     STP     3000
#define     EDP     18000


int main() {
    float       target = 0;
    char        buf[256];
    /*      CPA Attack Info     */
    int         trLen   = 24000;
    int         trNum   = 1000;
    int         curkey, maxkey;
    int         x, y;
    double      **trace;
    double      *Sx   = (double*)calloc(trLen, sizeof(double));
    double      *Sxx  = (double*)calloc(trLen, sizeof(double));
    double      *Sxy  = (double*)calloc(trLen, sizeof(double));
    double      *corr = (double*)calloc(trLen, sizeof(double));
    double      Sy, Syy;
    double      max, maxVal, maxLoc;
    float       input[IPN];
    float       fdum;
    float       flagPTR = (ENR - STR) / IPN;
    
    for(float i = 0, f = STR ; i <= IPN ; i++, f += flagPTR)
        input[(int)i] = f;
    
    FILE        *RFP = fopen(DIR, "rb");
    if(RFP == NULL) { puts("Failed to load trace!!\n"); return -1; }

    trace = (double**)calloc(trNum, sizeof(double*));
    for(int i = 0 ; i < trNum ; i++)
        trace[i] = (double*)calloc(trLen, sizeof(double*));

    for(int i = 0 ; i < trNum ; i++)
        fread(trace[i], sizeof(double), trLen, RFP);
    fclose(RFP);

    wt.p.mantissa = 0;
    wt.p.exponent = 0;
    wt.p.sign = 0;
    wt.num = 0;

    for(int i = 0 ; i < trNum ; i++) {
        for(int j = STP ; j < EDP ; j++) {
            Sx[i]  += trace[i][j];
            Sxx[i] += trace[i][j] * trace[i][j];
        }
    }
    printf("%lf\n", input[0]);

    for(int t = 0 ; t < 24 / bit ; t++) {
        max = 0;    maxVal = 0;     maxLoc = 0;

        for(curkey = 0 ; curkey < bit ; curkey++) { // get corr
            Sy = 0;
            Syy = 0;


            memset(Sxy,  0 , sizeof(double) * trLen);
            memset(corr, 0 , sizeof(double) * trLen);

            for(int i = 0 ; i < trNum ; i++) {
                ip.num = input[0];

                x = ip.p.mantissa;
                x += 0b00000000100000000000000000000000;
                y = x * (curkey + 128);
                
                if(t = 0) {
                    y = getMantissaHamming(y & 0x7f000000);
                    
                }
            }
        }
    }
    /*      get Correlation     */

    /*      Heap Free           */  
    free(Sx);
    free(Sxx);
    free(Sxy);
    free(corr);
    for(int i = 0 ; i < trNum ; i++)
        free(trace[i]);
    free(trace);
}