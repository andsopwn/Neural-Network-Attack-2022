#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

void test1() {
    float d[2] = {0, 0};
    float c[2] = {0.4, 0.84};
    float b[6];
    float a[6] = {-0.85, 0.62, -0.51, -0.31, 0.29, -0.45};
    for(int i = 0 ; i < 6 ; i++) {
        printf("%f  ", b[i] = hypertan(a[i] * 3));
    }
    puts("");

    d[0] += c[0] * (b[0] + b[1] + b[2] + b[3] + b[4] + b[5]);
    d[1] += c[1] * (b[0] + b[1] + b[2] + b[3] + b[4] + b[5]);
    printf("%f %f\n", hypertan(d[0]), hypertan(d[1]));
}

void test2() {
    printf("%f\n", hypertan(-3.468));
}

void test3() {
    float   wt = 1.355;
    
}

int main() {
    test1();
}