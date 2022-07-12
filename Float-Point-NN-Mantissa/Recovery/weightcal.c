#include <stdio.h>

int main() {
    float layer1[6] = { -0.85, 0.62, -0.51, -0.31, 0.29, -0.45 };
    float sum = 0;
    float wt = 1.625;
    for(int i = 0 ; i < 6 ; i++) {
        printf("%f\n", layer1[i] * wt);
    }
    printf("%f\n", sum);
}