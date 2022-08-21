#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE    *RFP;
    float   **data;
    int     trLen = 24000;
    int     trNum = 200;

    int     loc[200] = { 0, };

    RFP = fopen("relu.bin", "rb");
    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP); 
    for(int i = 0 ; i < trNum ; i++) {
        for(int j = 0 ; j < 5000 ; j++) {
            if(data[i][j] < -0.3) loc[i] = j; 
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