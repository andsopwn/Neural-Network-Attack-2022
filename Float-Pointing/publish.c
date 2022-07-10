#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void matlab(FILE *RFP) {
    FILE    *WFP;
    char    *p;
    char    buf[2048];
    int     data[128];
    int     cd[128];

    WFP = fopen("data.m", "w+t");
    fprintf(WFP, "%s", "TD = [ ");

    for(int m = 0 ; m < 128 ; m++) {
        fgets(buf, sizeof(buf), RFP);
        fgets(buf, sizeof(buf), RFP);

        for(int i = 0 ; i < 128 ; i++) {
            if(i == 0)  p = strtok(buf, " ");
            else        p = strtok(NULL, " ");
            data[i] = atoi(p);
        }

        for(int i = 0 ; i < 127 ; i++) cd[i] = data[i+1] - data[i];
        cd[127] = cd[126];
        
        for(int i = 0 ; i < 128 ; i++)
            fprintf(WFP, "%d ", cd[i]);
        fprintf(WFP, "%s", "; ");
        //fprintf(WFP, "[0x%02x]", m);

        fgets(buf, sizeof(buf), RFP);
    }

    fprintf(WFP, "%s", "];\n\n");
    fprintf(WFP, "%s\n", "save('CycleData', 'TD')");
    fprintf(WFP, "%s\n", "xlabel('Input')");
    fprintf(WFP, "%s\n", "ylabel('Weight')");
    fprintf(WFP, "%s\n", "colormap default");
    fprintf(WFP, "%s\n", "surf(TD)");

    fclose(WFP);

}

void header(FILE *RFP) {
    FILE    *WFP;
    char    buf[1024];
}

int main() {
    FILE    *RFP;
    char    *p;
    char    buf[1024];
    

    RFP = fopen("data.dat", "rt");
    if(RFP == NULL)     puts("FILE OPEN ERR");

    matlab(RFP);
    
    fclose(RFP);
}