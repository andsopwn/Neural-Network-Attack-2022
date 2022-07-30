#if 1
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

static void print_dec(char* string, uint8_t* src, uint32_t srclen) {
	printf("%s : ", string);
	for(int cnt_i = 0; cnt_i < srclen; cnt_i++)
		printf("%d ", src[cnt_i]);
	printf("\n");
}

int main()
{
	srand(time(NULL));

	uint8_t Public_A[TOY_N] = { 0x00, };
	uint8_t Public_B[TOY_N] = { 0x00, };
	uint8_t Secret_err[TOY_N] = { 0x00, };
	uint8_t Secret_key = 0;

	uint8_t CT_u = 0x00;
	uint8_t CT_v = 0x00;
	uint8_t msg = 0xCD;
	uint8_t recovered = 0xAB;

	uint8_t success_cnt = 0x00;
	uint8_t fail_cnt = 0x00;

	// Generate Param : [Public_A] and [Secret_err] and [Secret_key]
	// range of Public_A is [0, 78]
	// range of Secret_err and Secret_key is [0, 4]
	for (int cnt_i = 0; cnt_i < TOY_N; cnt_i++) {
		Public_A[cnt_i] = rand() % TOY_Q;
		Secret_err[cnt_i] = rand() % TOY_E + 1;
	}
	Secret_key = rand() % TOY_E;
	msg = rand() & 0x01;

	// [Key Pair]
	// Generating Public B := As + e  (Public_A * Secret_key + Secret_err)
	for (int cnt_i = 0; cnt_i < TOY_N; cnt_i++)
		Public_B[cnt_i] = (Public_A[cnt_i] * Secret_key + Secret_err[cnt_i]) % TOY_Q;

	// [Encryption]
	// Calculating CT_u := (sigma_Public A) mod TOY_Q 
	//             CT_V := ((sigma_Public B) + TOY_Q/2 * msg ) mod TOY_Q 
	for (int cnt_i = 0; cnt_i < TOY_N; cnt_i++) {
		CT_u += Public_A[cnt_i];
		CT_v += Public_B[cnt_i];
	}
	CT_v += floor(((TOY_Q) / 2)) * msg;
	CT_u %= TOY_Q;
	CT_v %= TOY_Q;

	// [Decryption]
	// Calculating CT_u := (sigma_Public A) mod TOY_Q 
	//             CT_V := ((sigma_Public B) + TOY_Q/2 * msg ) mod TOY_Q 
	uint8_t temp = 0x00;

	if (CT_v > Secret_key * CT_u)
		temp = CT_v - Secret_key * CT_u;
	else {
		temp = CT_v - Secret_key * CT_u +TOY_Q;
		temp %= TOY_Q;
	}

	if (temp <= TOY_Q / 2)
		recovered = 0;
	else
		recovered = 1;

	// [check param...]
	print_dec("[Public A]", Public_A, TOY_N);
	print_dec("[Secret E]", Secret_err, TOY_N);
	printf("[Secret K] = %d\n", Secret_key);
	print_dec("[Public B]", Public_B, TOY_N);
	printf("[Msg     ] = %d\n", msg);
	printf("[CT_u    ] = %d\n", CT_u);
	printf("[CT_v    ] = %d\n", CT_v);
	printf("[v-su    ] = %d\n", temp);
	printf("[recover ] = %d\n", recovered);

	return 0;
}


# endif