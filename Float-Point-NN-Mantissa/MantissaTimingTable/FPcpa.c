#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define     TraceNum        100
#define     TraceLength     24000
#define     startpt         0
#define     endpt           24000
#define     file    "mul100.bin"

int main() {
    FILE        *RFP;
    double      Sy, Syy;
    double      *Sx, *Sxx, *Sxy;
    double      *corr, maxcorr;
    double      a, b, c;
    float       **data;
    char        buf[256];
    int         i, j, k;
    int         key, maxkey;
    float       wt, ip, iv;

    /*      Model        */
    

    /*      POSIX       */
    if((RFP = fopen(file, "rb")) == NULL)     puts("\n\n[ RFP ] NULL ptr\n\n");
    data = (float**)calloc(TraceNum, sizeof(float*));
    for(i = 0 ; i < TraceNum ; i++) data[i] = (float*)calloc(TraceLength, sizeof(float));
    for(i = 0 ; i < TraceNum ; i++) fread(data[i], sizeof(float), TraceLength, RFP);
    fclose(RFP);

    /*      Memory       */
    corr    = (double*)calloc(TraceLength, sizeof(double));
	Sx      = (double*)calloc(TraceLength, sizeof(double));
	Sxx     = (double*)calloc(TraceLength, sizeof(double));
	Sxy     = (double*)calloc(TraceLength, sizeof(double));

    /*      Get Hamming         */
    for(i = 0 ; i < TraceNum ; i++) {
        for(j = startpt ; j < endpt ; j++) {
            Sx[j]   += data[i][j];
            Sxx[j]  += data[i][j] * data[i][j];
        }
    }

    for(i = 0 ; i < 1 ; i++) {
        maxcorr = 0;
        maxkey  = 0;
        for(key = 0 ; key < 256 ; key++) {
            Sy  = 0;
            Syy = 0;
            memset(Sxy, 0, sizeof(double) * TraceLength);
            for(j = 0 ; j < TraceNum ; j++) {
                wt = 1.3;

            }
        }
    }
    
    /*      Get Corr            */
    

    /*      Heap to NULL        */
    for(i = 0 ; i < TraceNum ; i++)     free(data[i]);
    free(data);
    free(corr);
    free(Sx);
    free(Sxx);
    free(Sxy);
}