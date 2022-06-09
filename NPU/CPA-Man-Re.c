#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "NNCPA.h"

ff  ip;
ff  wt;
ff  tt;

#define     DIR     "..//..//..//부채널연구//SCA_Experiment//a.npy"
//#define     DIR     "/Users/louxsoen/Documents/Univ/SCA_Experiment/a.npy"

#define     IPN     5000
#define     STR     -2.55
#define     ENR     2.55
#define     STP     0
#define     EDP     24000

#define     bitArea     3       // 비트 처리 단위 (3비트씩 나누어서 CPA)


int main() {
    char        buf[256];
    int         trLen   = 24000;
    int         trNum   = 5000;
    unsigned int    key, maxkey, maxLoc;
    unsigned int    x, y;
    unsigned int    hw;         // 해밍웨이트를 구할 임시 값
    unsigned char   hw_iv;      // 해밍웨이트를 구한 값
    double      **trace;
    double      *Sx   = (double*)calloc(trLen, sizeof(double));
    double      *Sxx  = (double*)calloc(trLen, sizeof(double));
    double      *Sxy  = (double*)calloc(trLen, sizeof(double));
    double      *corr = (double*)calloc(trLen, sizeof(double));
    double      Sy, Syy;
    double      max, maxVal;
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
    for(int i = 3 ; i < trNum ; i++) {
        for(int j = STP ; j < EDP ; j++) {
            Sx[j]  += trace[i][j];
            Sxx[j] += trace[i][j] * trace[i][j];
        }
    }

    int i, j, k, t;
    for(i = 0 ; i < 8 ; i++) {
        max = 0;    maxVal = 0;     maxLoc = 0;
        for(key = 1 ; key < 256 ; key++) { // get corr
            Sy = 0;     Syy = 0;
            
            memset(Sxy,  0 , sizeof(double) * trLen);
            memset(corr, 0 , sizeof(double) * trLen);

            for(j = 0 ; j < trNum ; j++) {
                ip.num = input[j];
                x = ip.p.mantissa;
                x += 0b00000000100000000000000000000000;
                //  1.m ip = 1.ex(1) + ip' / 128 | 1 * ip'/128
                y = x * (key + 128);
                hw = y;
                hw_iv = 0;
                if(i == 0) {
                    hw = hw & 0x7f000000;
                    for(t = 24 ; t < 31 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 1) {
                    hw &= 0x0fe00000;
                    for(t = 21 ; t < 28 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 2) {
                    hw &= 0x001fc00;
                    for(t = 18 ; t < 25 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 3) {
                    hw &= 0x003f800;
                    for(t = 15 ; t < 22 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 4) {
                    hw &= 0x00007f00;
                    for(t = 12 ; t < 19 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 5) {
                    hw &= 0x0000fe00;
                    for(t = 9 ; t < 16 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 6) {
                    hw &= 0x00001f00;
                    for(t = 6 ; t < 13 ; t++) { hw_iv += (hw >> t) & 1; }
                } 
                else if(i == 7) {
                    hw &= 0x000003f0;
                    for(t = 3 ; t < 10 ; t++) { hw_iv += (hw >> t) & 1; }
                }

                Sy  += hw_iv;
                Syy += hw_iv * hw_iv;

                for(k = STP ; k < EDP ; k++)
                    Sxy[k] += (double)(trace[j][k] * hw);
                    //print(seojun*STP);
            }

            for(int j = STP ; j < EDP ; j++) {
                a = (double)trNum * Sxy[j] - Sx[j] * Sy;
                b = sqrt((double)trNum * Sxx[j] - Sx[j] * Sx[j]);
                c = sqrt((double)trNum * Syy - Sy * Sy);
                
                corr[j] = a / (b * c);
                if(abs(corr[j] > max)) {
                    maxkey = key;
                    maxLoc = k;
                    max = fabs(corr[j]);
                }
            //printf("Sx %f \tSxx %f \tSy %f \tSyy %f \tSxy %f\n", Sx[w], Sxx[w], Sy, Syy, Sxy[w]);
            printf("%f\n", corr[j]);
            }
            fflush(stdout);
            if(key == 255)  printf("\n%d :::: maxCorr[%lf] | maxkey[%02X] | maxLoc[%d]\n", i, max, maxkey, maxLoc);
                            printf("%d :::: maxCorr[%lf] | maxkey[%02X] | maxLoc[%d] | [%02X]\r", i, max, maxkey, maxLoc, key);
            
            fflush(stdout);
        }
        
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

