#include <stdio.h>
#include <string.h>

#define         num     100

static float    wt   = 1.0;
static float    ip   = 1.0;

float mul(float weight, float input) {
    return  weight * input;
}

int main() {
    float   result[num];
    int     i;

    /*      Init        */
    for(i = 0 ; i < num ; i++) {
        result[i] = 0.0;
    }

    for(i = 0 ; i < num ; i++) {
        ip = 1.0 + 1.0/(float)num * i;
        result[i] += mul(wt, ip);
        printf("wt[%.2f] x ip[%.2f] = %.2f\n", wt, ip, result[i]);
    }

    wt += 1.0/(float)num;
}