#include <stdio.h>
#include "NNCPA.h"

int main() {
    float aa = 3.5919;
    ff a; a.num = aa;
    unsigned int x, y;

    printf("%d\n", a.p.exponent);
    printf("%d\n", a.p.mantissa);
    printf("%d\n", a.p.sign);

    printf("%d %d\n",getMantissaHamming(aa), getExponentHamming(aa));

    //x = a.p.mantissa;
    x  = 0b01000000010110011001100110011010;
    //  1.m ip = 1.ex(1) + ip' / 128 | 1 * ip'/128
    x += 0b00000000100000000000000000000000;
    for(int i = 31 ; i >= 0 ; i--) {
            if((x >> i) & 1 == 1) printf("1");
            else             printf("0");
        } puts("");
    y = x * (128 + 0);


    //y = x & 0x7f000000;
    //y = x & 0x0fe00000;
    y = x & 0x01fc0000;
    for(int i = 31 ; i >= 0 ; i--) {
        if((y >> i) & 1 == 1) printf("1");
        else             printf("0");
    } puts("");
}