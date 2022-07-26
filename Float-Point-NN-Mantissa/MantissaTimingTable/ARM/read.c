#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define     timingFN    "../../Trace/tttt.bin"

typedef struct FloatSave {
    float           val;
    float           abs;
    unsigned int    loc;
} fs;


void lowest() {
    FILE    *TFN;
    float   **data;
    float   F1, F2, max;
    
    unsigned int    diffloc[128] = { 0, };
    unsigned int    differr[128];
    unsigned int    difftime    = 0;
    unsigned int    cnt   = 0;
    unsigned int    density     = 128;
    unsigned int    samples     = 24000;
    fs  ff;

    data = (float**)calloc(sizeof(float*), density);
    for(int i = 0 ; i < density ; i++)
        data[i] = (float*)calloc(sizeof(float), samples);

    if((TFN = fopen(timingFN, "rb") ) == NULL)   { puts("STREAM ERR"); return; }
    
    for(int i = 0 ; i < density ; i++)
        fread(data[i], sizeof(float), samples, TFN);

    cnt = 0;
    for(int loc = 100 ; loc < 24000 ; loc++) {
        ff.loc = loc;
        ff.abs = fabs(data[119][loc]);
        if(cnt == 128)  break;
        if(ff.abs > 0.18 && ff.abs < 0.25) {
            diffloc[cnt] = loc + 1;
            cnt++;
        }
    }

    for(int i = 0 ; i < 128 ; i++) {
        printf("%d\n", diffloc[i]);
    }

    for(int i = 0 ; i < 127 ; i++) {
        printf("%d ", diffloc[i + 1] - diffloc[i]);
        if(i % 16 == 15) puts("");
    }    
    puts("");
    

    
    // ANALYSIS

    

    fclose(TFN);

    for(int i = 0 ; i < density ; i++)
        free(data[i]);
    free(data);
}

int main() {
    lowest();
}