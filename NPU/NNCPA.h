typedef union   ieee754 {
    float   num;
    struct {
        unsigned int    mantissa : 23;
        unsigned int    exponent : 8;
        unsigned int    sign     : 1;
    }   p;
}   ff;

void    prt(float a) {
    ff      one;
    one.num = a;

    printf("sign \t\t0x%x \nexponent \t0x%x\nmantissa \t0x%x\n", one.p.sign , one.p.exponent >> 1, one.p.mantissa);
}

int     getMantissaHamming(float a) {
    ff      one;    one.num = a;
    unsigned int    get = one.p.mantissa;
    unsigned int    hw  = 0;
    for(int i = 0 ; i < 24 ; i++)   hw += (get >> i) & 0b1;
    return hw;
}

int     getExponentHamming(float a) {
    ff      one;    one.num = a;
    unsigned int    get = one.p.exponent;
    unsigned int    hw  = 0;
    for(int i = 0 ; i < 10 ; i++)   hw += (get >> i) & 0b1;
    return hw;
}

int     getSignHamming(float a) {
    if(a >= 0)  return 0;
    else        return 1;
}

