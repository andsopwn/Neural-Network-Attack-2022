#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

float   weight[5] = { 0.25975, -0.208775, -0.0169609375, 0.295075, 0.00879296875 };
#define layerNum 3


typedef struct ND {
    float   in;
    float   *wt;
}   node;

typedef struct LD {
    int     num;    // Number of Node 
    node    *nd;    // Node Structer
}   layer;

void call(layer *L) {
    for(int i = 0 ; i < layerNum ; i++) {
        L[i].nd = (node*)calloc(sizeof(node), L[i].num);
        // 모든 레이어의 노드 동적할당
        for(int j = 0 ; j < L[i].num ; j++) {
            // 모든 레이어의 노드별 가중치 동적할당
            // ex) 1노드 - 5노드 : 1노드는 5개의 가중치를 가짐
            L[i].nd[j].wt = (float*)calloc(sizeof(float), L[i].num);
            // 모든 레이어의 모든 노드의 모든 가중치 0으로 초기화
            for(int k = 0 ; k < L[i].num ; k++)
                L[i].nd[j].wt[k] = 0;
        }
    }
}

void burn(layer *L) {
    // 모든 레이어의 노드 동적할당 해제
    for(int i = 0 ; i < layerNum ; i++) {
        for(int j = 0 ; j < L[i].num ; j++)
            free(L[i].nd[j].wt);
        // 모든 레이어의 노드 가중치 동적할당 해제
        free(L[i].nd);
    }
}

void Info_Prt(layer *L) {
    printf("\t :: %d개의 레이어 :: \n", layerNum);

    for(int i = 0 ; i < layerNum ; i++) {
        /////////////////////////////////////////////   In
        if(i == 0) printf("입력값\t\t");
        else       printf("현재값\t\t");
    
        for(int j = 0 ; j < L[i].num ; j++) {
            printf("[%.4f]\t\t", L[i].nd[j].in);
        } puts("");
        /////////////////////////////////////////////   O 
        printf("\t\t");
    
        for(int j = 0 ; j < L[i].num ; j++) {
            printf("   ○\t\t\t");
        } puts("");
        /////////////////////////////////////////////   Weight

        printf("\t\t");
        for(int j = 0 ; j < L[i].num ; j++) {
            printf("[%.4f]\t\t", L[i].nd[j].wt[0]);
        } puts("");

        puts("-----------------------------------------------------------------------------------------------------------------------------------");
        
    }
}

int main() {
    layer   L[layerNum];

    L[0].num = 5;
    L[1].num = 2;
    L[2].num = 3;

    call(L);


    Info_Prt(L);



    burn(L);
}