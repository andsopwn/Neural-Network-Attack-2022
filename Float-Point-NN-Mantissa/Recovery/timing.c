#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "highestcorr.h"

#define ND  152

float   rw[5] = { 1+(float)50/128, (float)10/128, (float)80/128, 1+(float)10/128, 1+(float)70/128 };

cr  inCPA(int bitloc) {
    cr      local, global;
    if(bitloc < 0 || bitloc > 3) { global.maxcorr = 0; global.maxloc = 0; global.maxwt = 0; return global; }

    FILE    *RFP, *YFP, *WFP;
    float   *corr;
    float   *cutX, *cutY;
    float   **data;
    int     trNum       = 5000;
    int     trLen       = 24000;
    int     wt, loc;
    unsigned char in8[4];
    unsigned int  in32;

    char    FN[100];
    char    YN[100];
    char    CN[100];

    sprintf(FN, "../Trace/%d.bin", ND); // trace
    sprintf(YN, "../Trace/%d.npy", ND); // input
    sprintf(CN, "../Trace/%d.crr", ND); // corr
    
    if((RFP = fopen(FN, "rt")) == NULL) puts("MAIN :: TRACE FILE IS NOT DETECTED");
    if((YFP = fopen(YN, "rt")) == NULL) puts("MAIN :: MODEL FILE IS NOT DETECTED");

    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP);
    
    cutX = (float*)calloc(sizeof(float), trNum);
    cutY = (float*)calloc(sizeof(float), trNum);
    corr = (float*)calloc(sizeof(double), trLen);

    init(&local, 0);
    for(int i = 0 ; i < trNum ; i++) {
        for(int j = 0 ; j < bitloc + 1 ; j++)
            fread(&in8, 1, sizeof(unsigned int), YFP);

        in32 = int32LE(in8);

        for(int k = 0 ; k < 32 ; k++)   
            cutY[i] += (in32 >> k) & 0b1;
            
        for(int j = 0 ; j < 3 - bitloc ; j++)
            fread(&in8, 1, sizeof(unsigned int), YFP);
    }
    fclose(YFP);

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
    else if(bitloc == 1)   WFP = fopen(CN, "a+b");
    else if(bitloc == 2)   WFP = fopen(CN, "a+b");

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

cr  teCPA() {
    cr      local, global;

    FILE    *RFP, *YFP, *WFP;
    float   *corr;
    float   *cutX, *cutY;
    float   **data;
    int     trNum       = 5000;
    int     trLen       = 24000;
    int     wt, loc;
    unsigned char in8[4];
    unsigned int  in32;
    float         f[3];
    float         fs;

    char    FN[100];
    char    YN[100];
    char    CN[100];

    sprintf(FN, "../Trace/%d.bin", ND); // trace
    sprintf(YN, "../Trace/%d.npy", ND); // input
    sprintf(CN, "../Trace/%d.crr", ND); // corr
    
    if((RFP = fopen(FN, "rt")) == NULL) puts("MAIN :: TRACE FILE IS NOT DETECTED");
    if((YFP = fopen(YN, "rt")) == NULL) puts("MAIN :: MODEL FILE IS NOT DETECTED");

    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP);
    
    cutX = (float*)calloc(sizeof(float), trNum);
    cutY = (float*)calloc(sizeof(float), trNum);
    corr = (float*)calloc(sizeof(float), trLen);

    init(&local, 0);
    fs = 0;
    for(int i = 0 ; i < trNum ; i++) {

        for(int k = 0 ; k < 3 ; k++) {
            fread(&in8, 1, sizeof(unsigned int), YFP);
            in32 = int32LE(in8);
            memcpy((unsigned char*)&f[k], (unsigned char*)&in32, 4);
            if(k == 2)  memcpy((unsigned char*)&f[k], (unsigned char*)&in32, 4);
        }
        fs = (f[0] + f[1] + f[2]) * 2;
        in32 = float_to_int32(fs);
        printf("%u\n", in32);
        return global;
        for(int k = 0 ; k < 32 ; k++)
            cutY[i] += (in32 >> k) & 0b1;
        fread(&in8, 1, sizeof(unsigned int), YFP);
    }
    fclose(YFP);

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
    WFP = fopen(CN, "w+b");

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
    cr    in[4];

    for(int i = 0 ; i < 1 ; i++) { in[i] = inCPA(i); printf("INPUT LOCATION(%d)\t[%d] (%.2f%%)\n", i, in[i].maxloc, in[i].maxcorr * 100); }
    //in[3] = teCPA(); printf("INPUT LOCATION(%d)\t[%d] (%.2f%%)\n", 0, in[3].maxloc, in[3].maxcorr * 100);
}