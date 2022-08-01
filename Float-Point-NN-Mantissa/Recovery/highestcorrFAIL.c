#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "highestcorr.h"

// a.bin - 주헌, f.bin - 서준
#if 0
    #define     trace    "a.bin"   
#else   
    #define     trace    "../Trace/152.bin"
    #define     input    "../Trace/152.npy"
    #define     multi    "../Trace/tim.bin"
#endif
        // tttt.bin
void highest() {
    FILE    *FN, *WF;
    cr      local, global;
    cr      *candi;
    int     rank[128];
    int     ranc[128];
    float   **data;
    float   **time;
    float   cutX[128], cutY[128];
    int     trNum       =    5000;
    int     trLen       = 24000;
    int     windowsize  = 200;
    int     stepsize    = 100;
    int     repeat      = (trLen - windowsize) / stepsize;

    float   curcorr;
    long double corrsum;
    int     wt, pos, k;
    //char    buf[256];

    // File Stream
    if((FN = fopen(trace, "rb")) == NULL)     puts("TRC OPEN ERR");
    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)   
        data[i] = (float*)calloc(sizeof(float), trLen);
    
    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, FN);
    fclose(FN);

    if((WF = fopen(multi, "rb")) == NULL)     puts("mul OPEN ERR");
    time = (float**)calloc(sizeof(float*), 128);
    for(int i = 0 ; i < 128 ; i++)   
        time[i] = (float*)calloc(sizeof(float), trLen);
    
    for(int i = 0 ; i < 128 ; i++)
        fread(time[i], sizeof(float), trLen, WF);
    fclose(WF);
    candi = (cr*)calloc(sizeof(cr), 128);

#if     debug == 0      // info
    puts("Setting Weight\t[1.2 1.6 | 1.0]\nSetting Input\t[1.4]\n");
    printf("repeat [%d] windowsize[%d] stepsize[%d]\n", repeat, windowsize, stepsize);
    printf("data[%lf]\n", data[1][23999]);

#elif   debug == 1      // trace correlation
    
    for(int i = 0 ; i < 128 ; i++)  init(&candi[i], 0);
    init(&global, 0);
    for(wt = 0 ; wt < 128 ; wt++) {
        init(&local, 0);
        // timing table model
        for(int i = 0 ; i < 128 ; i++)      cutY[i] = ((float)timing[wt][i]) / 100;
        for(pos = 0 ; pos < trLen - 4000 ; pos += stepsize) {            
            for(k = 0 ; k < 128 ; k++)  cutX[k] = data[1][k + pos];

            curcorr = fabs(correlation(cutX, cutY, 128));

            if(curcorr >= 1.0 || curcorr <= -1.0) { puts("Invalid Corr!!"); return; }
            if(curcorr > local.maxcorr) {
                local.maxcorr = curcorr;
                local.maxwt   = wt;
                local.maxloc  = pos;
            }
        }
        candi[wt] = local;
        if(local.maxcorr > global.maxcorr) 
            global = local;

    }
        //printf("%lf\n", curcorr);
    printf("MAX | C[%f]\tLoc[%d-%d]\tWT[%f](%d)\n", global.maxcorr, global.maxloc, global.maxloc + 128, (float)global.maxwt / 128 + 1, global.maxwt);


    for(int i = 0 ; i < 128 ; i++) {
        rank[i] = 1;
        for(int j = 0 ; j < 128 ; j++) {
            if(candi[i].maxcorr < candi[j].maxcorr) 
                rank[i]++;
        }
    }

    for(int i = 0 ; i < 128 ; i++)
        ranc[rank[i] - 1] = i;
    
    for(int i = 0 ; i < 10 ; i++) {
        printf("%d\tC[%f]\tLoc[%d-%d]\tWT[%f](%d)\n", i+1, candi[ranc[i]].maxcorr, candi[ranc[i]].maxloc, candi[ranc[i]].maxloc + 128, (float)candi[ranc[i]].maxwt / 128 + 1, candi[ranc[i]].maxwt);
    }

#endif
    for(int i = 0 ; i < trNum ; i++)   free(data[i]);
    for(int i = 0 ; i <   128 ; i++)   free(time[i]);
    free(data);
    free(time);
    free(candi);
}

int main() {
    highest();
    
}