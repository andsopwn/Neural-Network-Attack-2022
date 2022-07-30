#define KYBER_N 256     // c
#define KYBER_Q 3329

typedef struct {
    uint16_t coeffs[KYBER_N];
} poly;

void poly_base_schoolbook(poly *r, const poly *a, const poly *b);