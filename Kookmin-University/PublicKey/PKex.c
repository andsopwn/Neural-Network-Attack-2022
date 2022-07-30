#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define TOY_Q 79
#define TOY_N 4
#define TOY_E 2

typedef struct {
    uint16_t coeffs[256];
} poly;

typedef struct {
    poly vec[3];
} polyvec;

uint8_t PublicA[TOY_N] = { 0x00, };
uint8_t PublicB[TOY_N] = { 0x00, };
uint8_t Secret_err[TOY_N] = { 0x00, };
uint8_t Secret_key = 0;

int main() {
    
    for(int i = 0 ; i < TOY_N ; i++) {
        PublicA[i] = rand() % TOY_Q;
        Secret_err[i] = rand() % TOY_E + 1;
    }
    Secret_key = rand() % TOY_Q;
    uint8_t msg = rand() % 0x01;


    for(int i = 0 ; i < TOY_N ; i++) {
        PublicB[i] = PublicA[i] * Secret_key + 
    }
}