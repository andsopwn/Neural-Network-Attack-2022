#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "NNCPA.h"

ff  ip;
ff  wt;
ff  tt;

//#define     DIR     "..//..//..//부채널연구//SCA_Experiment//a.npy"
#define     DIR     "/Users/louxsoen/Documents/Univ/SCA_Experiment/a.npy"

#define     IPN     1000
#define     STR     -2.55
#define     ENR     2.55
#define     STP     0
#define     EDP     24000

#define     bitArea     3       // 비트 처리 단위 (3비트씩 나누어서 CPA)

int main() {
    float       target = 0;
    char        buf[256];
    int         trLen   = 24000;
    int         trNum   = 1000;
    int         key, maxkey;
    int         x, y;
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
    unsigned int    hw;         // 해밍웨이트를 구할 임시 값
    unsigned int    hw_iv;      // 해밍웨이트를 구한 값
    
    for(float i = 0, f = STR ; i <= IPN ; i++, f += flagPTR)
        input[(int)i] = f;
    
    FILE       *RFP = fopen(DIR, "rb");
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

    int j, k, i;
    for(int bitNum = 0 ; bitNum < 8 ; bitNum++) {
        max = 0;    maxVal = 0;     maxLoc = 0;
        for(key = 0 ; key < 128 ; key++) { // get corr
            Sy = 0;     Syy = 0;
            memset(Sxy,  0 , sizeof(double) * trLen);
            memset(corr, 0 , sizeof(double) * trLen);

            for(i = 0 ; i < trNum ; i++) {
                printf("%d");
                ip.num = input[0];
                x = ip.p.mantissa;
                x += 0x800000;
                //  1.m ip = 1.ex(1) + ip' / 128 | 1 * ip'/128
                y = x * (key + 128);

                if(bitNum == 0) {
                    hw &= 0x7f000000;
                    for(k = 24 ; k < 31 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 1) {
                    hw &= 0x0fe00000;
                    for(k = 21 ; k < 28 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 2) {
                    hw &= 0x0fe00000;
                    for(k = 18 ; k < 25 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 3) {
                    hw &= 0x01fc0000;
                    for(k = 15 ; k < 22 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 4) {
                    hw &= 0x0007f000;
                    for(k = 12 ; k < 19 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 5) {
                    hw &= 0x0000fe00;
                    for(k = 9 ; k < 16 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 6) {
                    hw &= 0x00001f00;
                    for(k = 6 ; k < 13 ; k++) { hw_iv += (hw >> i) & 0b1; }
                } 
                else if(bitNum == 7) {
                    hw &= 0x000003f0;
                    for(k = 3 ; k < 10 ; k++) { hw_iv += (hw >> i) & 0b1; }
                }

                Sy  += hw_iv;
                Syy += hw_iv * hw_iv;

                for(k = STP ; k < EDP ; k++)
                    Sxy[k] += (double)(trace[i][k] * hw);
                    //print(seojun*STP);
            }
        }

        for(int j = STP ; j < EDP ; j++) {
            a = (double)trNum * Sxy[j] - Sx[j] * Sy;
            b = sqrt((double)trNum * Sxx[j] - Sx[j] * Sx[j]);
            c = sqrt((double)trNum * Syy - Sy * Sy);
            
            corr[j] = a / (b * c);
            if(abs(corr[j] > max)) {
                maxkey = key;
                maxLoc = j;
                max = fabs(corr[j]);
            }
            fflush(stdout);
            printf("maxLoc[%f] maxkey[%02X] maxbit[%02X] Corr[%lf]\n", maxLoc, maxkey, maxkey & 0x70, max);
        }
        //fflush(stdout);
        //printf("maxCorr[%lf] | maxkey[%d] | maxLoc[%lf]\n", max, maxkey, maxLoc);


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

