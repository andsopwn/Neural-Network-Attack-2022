#include <stdio.h>


int main() {
    typedef union con{
        float f;
        unsigned int u;
    } ff;

    ff f;
    f.f = 3.4;
    printf("%08X\n", f.u);
}