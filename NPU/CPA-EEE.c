#include <stdio.h>
#include <math.h>

float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

void test1() {
    float   ip = 0.8;
    float   wt = 1.0391;
    for(int i = 0 ; i < 100 ; i++) {
        printf("%f ", hypertan(wt * (ip + (float)i/100)));
        if(i % 16 == 15) puts("");
    }
    puts("");
}

void test2() {
    float   a = 1;
    float   b = 3.99999999999999;
    float   c;
    printf("%.23f\n", c = hypertan(a * b));
    printf("%.23f\n", hypertan(c));
}

int main() {
    test2();
}