#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE    *wfp;
    float   temp;
    wfp = fopen("randomValue.print", "w+b");
    for(int i = 0 ; i < 30000 ; i++) {
        temp = (float)(rand() % 255) / 100;
        if((rand() % 4) == 1)   temp *= -1; 
        fprintf(wfp, "%.2f, ", temp);
    }

    fclose(wfp);
}