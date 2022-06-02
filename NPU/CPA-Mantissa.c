#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef union   ieee754 {
    float   num;
    struct {
        unsigned int    mantissa : 23;
        unsigned int    exponent : 8;
        unsigned int    sign     : 1;
    }   p;
}   ff;

ff  input;
ff  weight;
ff  test;

int main() {
    ff  one;
    one.num = 2.3;
    printf("sign \t\t0x%x \nexponent \t0x%x\nmantissa \t0x%x\n", one.p.sign , one.p.exponent >> 1, one.p.mantissa);
}