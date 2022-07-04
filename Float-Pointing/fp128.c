#include <stdio.h>
#include <string.h>

#define         num     128

static float    wt   = 1.0;
static float    ip   = 1.0;

float mul(float weight, float input) {
    return  weight * input;
}

int main() {
    float   result[num];
    float   unit = 1/(float)128;
    int     i;

    /*      Init        */
    for(i = 0 ; i < num ; i++) {
        result[i] = 0.0;
    }

    for(i = 0 ; i < num ; i++) {
        result[i] = mul(wt, ip);
        printf("wt[%.2f] x ip[%.2f] = %.2f\n", wt, ip, result[i]);
        ip += unit;
    }

    wt += 1.0/(float)num;
}