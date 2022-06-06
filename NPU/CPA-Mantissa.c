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
#define     STP     0
#define     EDP     24000


int main() {
    float       target = 0;
    char        buf[256];
    /*      CPA Attack Info     */
    int         trLen   = 24000;
    int         trNum   = 1000;
    int         curkey, maxkey;
    int         x, y;
    int         hw;
    double      **trace;
    double      *Sx   = (double*)calloc(trLen, sizeof(double));
    double      *Sxx  = (double*)calloc(trLen, sizeof(double));
    double      *Sxy  = (double*)calloc(trLen, sizeof(double));
    double      *corr = (double*)calloc(trLen, sizeof(double));
    double      Sy, Syy;
    double      max, maxVal, maxLoc;
    double      a, b, c;
    float       input[IPN + 1];
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

    /*      MODEL Sx      */
    for(int i = 0 ; i < trNum ; i++) {
        for(int j = STP ; j < EDP ; j++) {
            Sx[i]  += trace[i][j];
            Sxx[i] += trace[i][j] * trace[i][j];
        }
    }

    printf("%lf\n", input[0]);

    int t, j, k, i;
    for(t = 0 ; t < 24 / bit ; t++) {
        max = 0;    maxVal = 0;     maxLoc = 0;

        for(curkey = 0 ; curkey < bit ; curkey++) { // get corr
            Sy = 0;
            Syy = 0;


            memset(Sxy,  0 , sizeof(double) * trLen);
            memset(corr, 0 , sizeof(double) * trLen);

            for(i = 0 ; i < trNum ; i++) {
                ip.num = input[0];
                x = ip.p.mantissa;
                //  1.m ip = 1.ex(1) + ip' / 128 | 1 * ip'/128
                x += 0b00000000100000000000000000000000;
                
                y = x * (curkey + 128);

                hw = 0;
                y >>= 8;
                for(j = 0 ; j < 8 ; j++)
                    hw += (y >> j) & 1;


                Sy  += hw;
                Syy += hw * hw;

                for(k = STP ; k < EDP ; k++)
                    Sxy[k] += (double)(trace[i][k] * hw);
            }
        }

        for(int j = STP ; j < EDP ; j++) {
            a = (double)trNum * Sxy[j] - Sx[j] * Sy;
            b = sqrt((double)trNum * Sxx[j] - Sx[j] * Sx[j]);
            c = sqrt((double)trNum * Syy - Sy * Sy);
            
            corr[j] = a / (b * c);
            if(fabs(corr[j] > max)) {
                maxkey = curkey;
                maxLoc = j;
                max = fabs(corr[j]);
            }
        }
        fflush(stdout);
        printf("maxCorr[%lf] | maxkey[%d] | maxLoc[%lf]\n", max, maxkey, maxLoc);

    }


    /*      Heap Free           */  
    free(Sx);
    free(Sxx);
    free(Sxy);
    free(corr);
    for(int i = 0 ; i < trNum ; i++)
        free(trace[i]);
    free(trace);
}

