#include <stdio.h>

#define     numX    3
#define     numY    6
#define     NumZ    2

float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

typedef struct Node {
    float   weight;
    float   z;
    
}   nd;
int main() {
    nd      yLayer[HL];

    for(int i = 0 ; i < HL ; i++)   yLayer[i].weight = 1.0;
    

}