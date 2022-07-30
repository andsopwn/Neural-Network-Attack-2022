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

float char_to_float() {
    char    pt[16] = {0x82, 0x59, 0x0A, 0x0B, 0xE5, 0x29, 0xE9, 0x9A, 0x39, 0xAD, 0x52, 0x93, 0x45, 0x45, 0x08, 0xC2};

    float    f[3];
    unsigned int a[3];
    

    a[0] = pt[0] & 0xff | (pt[1] << 8) & 0x0000ff00 | (pt[ 2] << 16) & 0x00ff0000 | (pt[ 3] << 24) & 0xff000000;
    a[1] = pt[4] & 0xff | (pt[5] << 8) & 0x0000ff00 | (pt[ 6] << 16) & 0x00ff0000 | (pt[ 7] << 24) & 0xff000000;
    a[2] = pt[8] & 0xff | (pt[9] << 8) & 0x0000ff00 | (pt[10] << 16) & 0x00ff0000 | (pt[11] << 24) & 0xff000000;
    //a[0] = pt[0] | (pt[1] << 8) | (pt[ 2] << 16) | (pt[ 3] << 24);
    //a[1] = pt[4] | (pt[5] << 8) | (pt[ 6] << 16) | (pt[ 7] << 24);
    //a[2] = pt[8] | (pt[9] << 8) | (pt[10] << 16) | (pt[11] << 24);
    //printf("%x %x %x\n", a[0], a[1], a[2]);
    
    memcpy((unsigned char*)&f[0], (unsigned char*)&a[0], 4);
    memcpy((unsigned char*)&f[1], (unsigned char*)&a[1], 4);
    memcpy((unsigned char*)&f[2], (unsigned char*)&a[2], 4);
    //layer[0].nd[0].output = 1+(float)65/128;
    //layer[0].nd[1].output = 1+(float)15/128;
    //layer[0].nd[2].output = 1+(float)80/128;a[0] = pt[0] | (pt[1] << 8) | (pt[ 2] << 16) | (pt[ 3] << 24);
    //a[1] = pt[4] | (pt[5] << 8) | (pt[ 6] << 16) | (pt[ 7] << 24);
    //a[2] = pt[8] | (pt[9] << 8) | (pt[10] << 16) | (pt[11] << 24);
    
    printf("%.50f %.50f %.50f\n", f[0], f[1], f[2]);

    return f[0];
}

unsigned int float_to_int32(float f) {
    float a = 3.2f;

    unsigned int b = (unsigned int)a;

    printf("%08x\n", b);
}

int main() {
    float k = char_to_float();
    float_to_int32(k);
}