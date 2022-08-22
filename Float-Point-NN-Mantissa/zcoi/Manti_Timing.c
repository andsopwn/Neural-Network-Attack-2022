#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE    *RFP;
    float   **data;
    int     trLen = 5000;
    int     trNum = 16384;

    int     loc[16384] = { 0, };

    RFP = fopen("multiply.bin", "rb");
    data = (float**)calloc(sizeof(float*), trNum);
    for(int i = 0 ; i < trNum ; i++)
        data[i] = (float*)calloc(sizeof(float), trLen);

    for(int i = 0 ; i < trNum ; i++)
        fread(data[i], sizeof(float), trLen, RFP);
    fclose(RFP); 

    for(int i = 0 ; i < trNum ; i++) {
        for(int j = 1500 ; j < 4000 ; j++) {
            if(data[i][j] < -0.3) loc[i] = j; 
        }
    }
    
    RFP = fopen("mat", "w+t");
    fprintf(RFP, "%s", "multime = [ ");

    for(int i = 0 ; i < 128 ; i++) {
        for(int k = 0 ; k < 128 ; k++) {
            if(k != 127) fprintf(RFP, "%d, ", loc[i * 128 + k]);
            else         fprintf(RFP, "%d ",  loc[i * 128 + k]);
        } 
        if(i != 127)     fprintf(RFP, "%s", "; ");
        else             { }

    }

    fprintf(RFP, "%s", "]\n");
    fclose(RFP);


    for(int i = 0 ; i < trNum ; i++)
        free(data[i]);
    free(data);
}