#include <stdio.h>
#include "8bit.h"

int main() {
    printf("TD = [ ");
    for(int i = 0 ; i < 128 ; i++) {
        for(int k = 0 ; k < 128 ; k++)
            printf("%d ", timing[i][k]);
        printf("; ");
    }
    printf("];\n");
}