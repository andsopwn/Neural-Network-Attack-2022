#include <stdio.h>
#include <math.h>
#include <string.h>

// 3 6 2

#define     INPUT   3   // 입력층 노드 개수
#define     HIDDEN  6   // 첫번째 은닉층 노드 개수 [1]
#define     OUTPUT  2   // 출력층 노드 개수

float sigmoid(float h) { return (1 / (1 + exp((-1)*h))); }
float tanhh(float h)    { return (2 / (1 + exp(-2 * h)) - 1); }
float relu(float h)    { return (h > 0 ? h : 0); }

int main() {
/*              Forward Propagation             */
    int     i, j;
    float   e = 0.1;    // 학습률
    float   u = 0;      // 논리합
    float   L0[INPUT][HIDDEN];     // Weight Of Input Layer to Hidden Layer[1]
    float   L1[HIDDEN][OUTPUT];    // Weight Of Hidden Layer[1] to Output Layer
    float   SL0[HIDDEN];           // Sum Of Weight Input Layer to Hidden Layer[1]
    float   SL1[OUTPUT];           // Sum Of Weight Hidden Layer[1] to Output Layer
    float   AFR[HIDDEN];           // Result of Activation Function
    float   X[INPUT] = { 1, 2, 3 };    // Input Data
    float   Y[OUTPUT];             // Output Data

    // Weight Setting
    for(i = 0 ; i < INPUT ; i++) {
        for(j = 0 ; j < HIDDEN ; j++)
            L0[i][j] = 1.0;
    }
    for(i = 0 ; i < HIDDEN ; i++) {
        for(j = 0 ; j < OUTPUT ; j++)
            L1[i][j] = 1.0;
    }
    for(i = 0 ; i < HIDDEN ; i++)
        SL0[i] = 0;
    for(i = 0 ; i < OUTPUT ; i++) 
        SL1[i] = 0;

/*              Input Layer - Hidden[1] Layer             */
    for(i = 0 ; i < HIDDEN ; i++) {
        for(j = 0 ; j < INPUT ; j++)
            SL0[i] += X[j] * L0[j][i];

        AFR[i] = relu(SL0[i]);
        //AFR[i] = relu(SL0[i]);
        //AFR[i] = tanh(SL0[i]);
    }
    printf("Activation Function First Layer\n");
	for(i = 0 ; i < HIDDEN ; i++)
	    printf("%d : %f \t %f\n", i + 1, AFR[i], L0[2][i]);
    puts("");

/*              Hidden[1] Layer - Output Layer            */
    for(i = 0 ; i < OUTPUT ; i++) {
        for(j = 0 ; j < HIDDEN ; j++)
            SL1[i] = AFR[j] * L1[j][i];
        
        Y[i] = exp(SL1[i]);
        u += Y[i];
    }

    for (i = 0; i < OUTPUT; i++)
		Y[i] /= u;

    printf("Result\n");
	for (i = 0; i < OUTPUT; i++)
		printf("%d : %f \n", i + 1, Y[i]);
	printf("\n");
}