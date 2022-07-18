//*********************************************************//
// 	   MLP : 입력(X) : 3 / 은닉(H1) : 6  / 출력(Y) : 2	   //
//*********************************************************//

// 가중합 연산 직후 활성함수 연산_Ver

#include <stdio.h>
#include <math.h>
#include <string.h>

#define INPUT 1		// 입력층 노드 개수 
#define HIDDEN 1	// 첫번째 은닉층 노드 개수 
#define OUTPUT 1	// 출력층 노드 개수 

// 입,출력 데이터 
float X[INPUT] = { 1.5 };  // ※ Setting ※
float Y[OUTPUT];

// Weight ( ※ Setting ※ )
float WX[INPUT][HIDDEN]; // 입력층 - 은닉층(1) 가중치(Weight) 
float WH[HIDDEN][OUTPUT]; // 은닉층(1) - 출력층 가중치(Weight)	

						  // Weight Sum
float WSX[HIDDEN]; // 입력값으로 계산한 은닉층(1)에 들어갈 가중합
float WSH[OUTPUT]; // 은닉층(1)의 출력값으로 계산한 출력층에 들어갈 가중합 

				   // Activefunction Result
float AFR[HIDDEN];

// Active Function Implement
float sigmoid(float h) {    return (1 / (1 + exp((-1)*h)));     }
float tanh2(float h)   {    return (2 / (1 + exp(-2 * h)) - 1); }
float relu(float h)    {    return (h > 0 ? h : 0);     }


int main()
{
	//*********************************************************//
	// 				 순전파 (Forward Propagation)	           //
	//*********************************************************//

	int i, j;
	float sum = 0;

	// Weight 설정 
	for (i = 0; i < INPUT; i++) {
		for (j = 0; j < HIDDEN; j++)
			WX[i][j] = 1.7;
	}

	for (i = 0; i < HIDDEN; i++) {
		for (j = 0; j < OUTPUT; j++)
			WH[i][j] = 1.0;
	}


	// 초기화 
	memset(WSX, 0, sizeof(WSX));
	memset(WSH, 0, sizeof(WSH));

	//*********************************************************//
	// 				      입력층 - 은닉층             	       //
	//*********************************************************//
	// 가중합 + 활성함수 
	for (i = 0; i < HIDDEN; i++)
	{
		for (j = 0; j < INPUT; j++)  
			WSX[i] += X[j] * WX[j][i];
		//AFR[i] = sigmoid(WSX[i]);
		//AFR[i] = tanh2(WSX[i]);
		AFR[i] = tanh2(WSX[i]);
	}

	printf("\n%lf\n", WSX[0]);

	// 출력
#if 1
	printf("<< Active Function 1(AFR) >> \n");
	for (i = 0; i < HIDDEN; i++)
		printf("%d : %f \n", i + 1, AFR[i]);
    puts("");
#endif

	//*********************************************************//
	// 				      은닉층 - 출력층             	       //
	//*********************************************************//
	// 가중합 + softmax exp연산 
	for (i = 0; i < OUTPUT; i++) {
		for (j = 0; j < HIDDEN; j++)  {
			WSH[i] += AFR[j] * WH[j][i];
		}
		//Y[i] = exp(WSH[i]); 
        Y[i] = WSH[i]; 
		sum += Y[i];
	} 
	for (i = 0; i < OUTPUT; i++)
		Y[i] /= sum; 

	printf("\n\ntest : %f\n\n", tanh2(1.8));
	// 출력 
#if 1
	printf("<< Result (Y) >> \n");
	for (i = 0; i < OUTPUT; i++)
		printf("%d : %f \t%f\n", i + 1, Y[i], WSH[i]);
    puts("");
    printf("%f\n", sum);
#endif

	return 0;
}
