#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test1() {
    float layer1[6] = { -0.85, 0.62, -0.51, -0.31, 0.29, -0.45 };
    float sum = 0;
    float wt = 1.625;
    for(int i = 0 ; i < 6 ; i++) {
        printf("%f\n", layer1[i] * wt);
    }
    printf("%f\n", sum);
}

unsigned int   int32LE(unsigned char val[4]) {
    return val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24) ;
}

void test3() {
    FILE *FN;
    unsigned char tmp[4];
    unsigned int a;
    float f;

    FN = fopen("this.npy", "rt");
    fread(&tmp, 1, sizeof(float), FN);
    fclose(FN);

    printf("%x %x %x %x\t", tmp[0], tmp[1], tmp[2], tmp[3]);

    a = int32LE(tmp);
    memcpy((unsigned char*)&f, (unsigned char*)&a, 4);
    printf("%.32f", f);
}

int main() {
    test3();
}