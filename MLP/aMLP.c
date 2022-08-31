#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

#define numInputs       5
#define numHiddenNodes  3
#define numOutputs      2

float   inputLayer[numInputs];
////////////////////////////////////
float   hiddenLayer[numHiddenNodes];
float   hiddenLayerBias[numHiddenNodes];
float   outputLayer[numOutputs];
//float   outputLayerBias[numOutputs];
float   hiddenWeights[numInputs][numHiddenNodes];
float   outputWeights[numHiddenNodes][numOutputs];

void prtInfo() {
    
    {
    printf("\t\t   :: Network Result ::\n");
    printf("\t  INPUT\t\t HIDDEN\t\t OUTPUT\n");
    printf("\t%f\t%f\t\t\n", inputLayer[0], hiddenLayer[0]);
    printf("\t%f\t\t\t%f\n", inputLayer[1], outputLayer[0]);
    printf("\t%f\t%f\t\t\n", inputLayer[2], hiddenLayer[1]);
    printf("\t%f\t\t\t%f\n", inputLayer[3], outputLayer[1]);
    printf("\t%f\t%f\t\t\n", inputLayer[4], hiddenLayer[2]);
    }    
}
void init(float a) {
    for(int i = 0 ; i < numHiddenNodes ; i++) {
        hiddenLayer[i]      = a;
        hiddenLayerBias[i]  = a;
        for(int j = 0 ; j < numOutputs ; j++)
            outputWeights[i][j] = a;
    }

    for(int i = 0 ; i < numInputs ; i++) {
        inputLayer[i]   = a;
        for(int j = 0 ; j < numHiddenNodes ; j++)
            hiddenWeights[i][j] = a;
    }

    for(int i = 0 ; i < numOutputs ; i++)
        outputLayer[i]  = a;
}

void setVal() {
    init(0);
    // Hidden Weight
    hiddenWeights[0][0] = 0.25975;
    hiddenWeights[0][1] = 0;
    hiddenWeights[0][2] = 0;
    hiddenWeights[1][0] = -0.208775;
    hiddenWeights[1][1] = 0;
    hiddenWeights[1][2] = 0;
    hiddenWeights[2][0] = -0.0169609375;
    hiddenWeights[2][1] = 0;
    hiddenWeights[2][2] = 0;
    hiddenWeights[3][0] = 0.295075;
    hiddenWeights[3][1] = 0;
    hiddenWeights[3][2] = 0;
    hiddenWeights[4][0] = 0.00879296875;
    hiddenWeights[4][1] = 0;
    hiddenWeights[4][2] = 0;
    hiddenLayerBias[0]  = -50.8992;
    hiddenLayerBias[1]  = -50.8992;
    hiddenLayerBias[2]  = -50.8992;
    // Output Weight
    outputWeights[0][0] = 1;
    outputWeights[0][1] = 1;
    outputWeights[1][0] = 1;
    outputWeights[1][1] = 1;
    outputWeights[2][0] = 1;
    outputWeights[2][1] = 1;
}

void calVal() {
    for(int i = 0 ; i < numHiddenNodes ; i++) {
        for(int j = 0 ; j < numInputs ; j++) {
            hiddenLayer[i] += inputLayer[j] * hiddenWeights[j][i];
        }
        hiddenLayer[i] += hiddenLayerBias[i];
    }

    for(int i = 0 ; i < numHiddenNodes ; i++)
        hiddenLayer[i] = (hiddenLayer[i]);
    
    for(int i = 0 ; i < numOutputs ; i++) {
        for(int j = 0 ; j < numHiddenNodes ; j++) {
            outputLayer[i] += hiddenLayer[j] * outputWeights[j][i];
        }
    }
}

int main() {
    setVal();
    inputLayer[0] = 2;
    inputLayer[1] = 0;
    inputLayer[2] = 255;
    inputLayer[3] = 0;
    inputLayer[4] = 0;
    calVal();
    prtInfo();
}