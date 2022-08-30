#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE    *RFP;
    float   **data;
    int     trLen = 5000;
    int     trNum = 100;

    int     loc[100] = { 0, };

    RFP = fopen("../Trace/relu10.bin", "rb");
    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP); 
    for(int i = 0 ; i < trNum ; i++) {
        for(int j = 800 ; j < 4000 ; j++) {
            if(data[i][j] < -0.2) loc[i] = j; 
        }
    }
    
    printf("c = [ ");
    for(int i = 0 ; i < trNum ; i++)
        printf("%d , ", loc[i]);
    printf("]\n");



    for(int i = 0 ; i < trNum ; i++)
        free(data[i]);
    free(data);
}