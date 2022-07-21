#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE    *wfp;
    float   temp;
    int     repeat = 3000;
    srand(time(NULL));
    wfp = fopen("mlptiming.h", "w+b");
    fprintf(wfp, "float mlptiming[%d] = {\n\t", repeat);
    for(int i = 0 ; i < repeat ; i++) {
        temp = (float)(rand() % 255) / 100;
        if((rand() % 4) == 1)   temp *= -1; 
        fprintf(wfp, "%.2f, ", temp);
    }
    fprintf(wfp, "\t};");

    fclose(wfp);
}