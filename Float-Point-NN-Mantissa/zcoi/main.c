#include <stdio.h>

float   weight[5] = { 0.25975, -0.208775, -0.0169609375, 0.295075, 0.00879296875 };

unsigned int float_to_int32(float f) {
    union conv {
        float f;
        unsigned int a;
    };
    union conv ff;
    ff.f = f;
    //printf("%08x\n", ff.a);
    return ff.a;
}

void test() {
    float   p = (float)1 * weight[0];
    unsigned int a = float_to_int32(p);
    printf("%08X\n", a);
    int hw = 0 ;
    for(int i = 0 ; i < 32 ; i++)
    hw += (a >> i) & 0b1;
    printf("%d", hw);

    printf("%f", (float)a);
}
void test1() {
    float now = 50;
    printf("%f", now);
}

int main() {
    test1();
}   