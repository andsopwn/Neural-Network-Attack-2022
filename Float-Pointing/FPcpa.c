#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define     TraceNum        30
#define     TraceLength     24000
#define     file    "multiplication.bin"

int main() {
    FILE        *RFP;
    double      Sy, Syy;
    double      *Sx, *Sxx, *Sxy;
    double      *corr, maxCorr;
    double      a, b, c;
    float       **data;
    char        buf[256];
    int         i, j, k;

    /*      POSIX       */
    if((RFP = fopen(file, "rb")) == NULL)     puts("\n\n[ RFP ] NULL ptr\n\n");
    
    data = (float**)calloc(TraceNum, sizeof(float*));
    for(i = 0 ; i < TraceNum ; i++)
        data[i] = (float*)calloc(TraceLength, sizeof(float));
    for(i = 0 ; i < TraceNum ; i++)
        fread(data[i], sizeof(float), TraceLength, RFP);
    fclose(RFP);


    printf("%f\n", data[0][0]);
    /*     Generate Model       */
    

    /*     S TO H     */
    corr    = (double*)calloc(TraceLength, sizeof(double));
	Sx      = (double*)calloc(TraceLength, sizeof(double));
	Sxx     = (double*)calloc(TraceLength, sizeof(double));
	Sxy     = (double*)calloc(TraceLength, sizeof(double));

   


    /* Heap to NULL */
    for(i = 0 ; i < TraceNum ; i++)     free(data[i]);
    free(data);
    free(corr);
    free(Sx);
    free(Sxx);
    free(Sxy);
}