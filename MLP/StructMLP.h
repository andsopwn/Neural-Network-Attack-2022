#define     LM    3

float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

typedef struct Node {
    float   weight;
    float   output;
}   nd;

typedef struct Layer {
    int     num;      // 노드의 개수
    nd      *nd;      
}   ld;