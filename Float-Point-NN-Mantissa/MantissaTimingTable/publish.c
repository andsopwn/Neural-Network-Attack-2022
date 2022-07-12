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
    fprintf(WFP, "%s\n", "colormap default");
    fprintf(WFP, "%s\n", "surf(TD)");
    fprintf(WFP, "%s\n", "xlabel('Input')");
    fprintf(WFP, "%s\n", "ylabel('Weight')");

    fclose(WFP);

}

void header(FILE *RFP) {
    FILE    *WFP;
    char    *p;
    char    buf[2048];
    int     data[128];
    int     cd[128];

    WFP = fopen("data.h", "w+t");

    fprintf(WFP, "%s", "unsigned int timing[128][128] = {\n");

    for(int i = 0 ; i < 128 ; i++) {
        fprintf(WFP, "%s%d[0x%02X]%s", "\t{\t// weight = ", i, i, "\n\t\t");
        
        fgets(buf, sizeof(buf), RFP);
        fgets(buf, sizeof(buf), RFP);

        for(int j = 0 ; j < 128 ; j++) {
            if(j == 0)  p = strtok(buf, " ");
            else        p = strtok(NULL, " ");
            data[j] = atoi(p);
        }
        for(int j = 0 ; j < 127 ; j++) cd[j] = data[j+1] - data[j];
        cd[127] = cd[126];
        
        for(int j = 0 ; j < 128 ; j++) {
            if(j == 127) fprintf(WFP, "%d", cd[j]);
            else fprintf(WFP, "%d, ", cd[j]);
        }
        if(i == 127) fprintf(WFP, "%s", "\n\t}");
        else fprintf(WFP, "%s", "\n\t}, ");
        fgets(buf, sizeof(buf), RFP);

    }

    fprintf(WFP, "%s", "\n};");

    fclose(WFP);
}

int main() {
    FILE    *RFP;
    char    *p;
    char    buf[1024];
    

    RFP = fopen("data.dat", "rt");
    if(RFP == NULL)     puts("FILE OPEN ERR");

    //matlab(RFP);
    header(RFP);

    fclose(RFP);
}