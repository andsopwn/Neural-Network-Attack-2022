#include <stdio.h>
#include <stdint.h>
#include "Kyber.h"

void poly_base_schoolbook(poly *r, const poly *a, const poly *b) {
    uint16_t    temp[KYBER_N * 2];

    for(int i = 0 ; i < KYBER_N - 1 ; i++)
        r->coeffs[i] = 0;


    for(int i = 0 ; i < KYBER_N - 1 ; i++) {
        for(int j = 0 ; j < KYBER_N - 1 ; j++) {
            temp[i + j] += a->coeffs[i] * b->coeffs[j];
            temp[i + j] = temp[i + j] % KYBER_Q;
        }
    }
    for(int i = KYBER_N ; i < KYBER_N * 2 ; i++)
        r->coeffs[i - KYBER_N] += (temp[i - KYBER_N] - temp[i]) % KYBER_Q;
}

int main(void) {
    poly a, b, r;

    for(int i = 0 ; i < KYBER_N ; i++) 
        a.coeffs[i] = i + 1;

    for(int i = 0 ; i < KYBER_N ; i++)
        b.coeffs[i] = i * 2;

    poly_base_schoolbook(&r, &a, &b);

    for(int i = 0 ; i < KYBER_N ; i++)
        printf("a[%d] = %d\t b[%d] = %d\t r[%d] = %d \n", i , a.coeffs[i], i, b.coeffs[i], i, r.coeffs[i]);

}