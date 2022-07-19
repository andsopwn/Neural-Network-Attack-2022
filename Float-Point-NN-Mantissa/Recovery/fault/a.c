#include <stdio.h>

float X[3] = { 1+(float)49/128, 1+(float)56/128, 1+(float)89/128 };

int main() {
    printf("%f %f %f", X[0], X[1], X[2]);
}