#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define     timingFN    "../../Trace/tttt.bin"

void lowest() {
    FILE    *TFN;
    float   **data;

    unsigned int    density = 128;
    unsigned int    samples = 24000;

    data = (float**)calloc(sizeof(float*), density);
    for(int i = 0 ; i < density ; i++)
        data[i] = (float*)calloc(sizeof(float), samples);


    if((TFN = fopen(timingFN, "rb") ) == NULL)   { puts("STREAM ERR"); return; }
    
    for(int i = 0 ; i < density ; i++)
        fread(data[i], sizeof(float), samples, TFN);
    // ANALYSIS

    

    fclose(TFN);

    for(int i = 0 ; i < density ; i++)
        free(data[i]);
    free(data);
}

int main() {
    lowest();
}