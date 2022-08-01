#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "StructMLP.h"

void weight_lprint(ld *Layer) {
    for(int i = 0 ; i < LAYERNUM ; i++) {
        fflush(stdout);
        printf("Layer[%d]\tWeight ", i);
        for(int k = 0 ; k < Layer[i].num ; k++)     printf("\t[%.2f]\t", Layer[i].nd[k].weight);
        printf("(%d Nodes)\n\t\t------ ", Layer[i].num);
        for(int k = 0 ; k < Layer[i].num ; k++)     printf("\t%f", Layer[i].nd[k].output);
        printf("\t-> (out)\n");
    }
    puts("");
}

void weight_initialization(ld *Layer, float setting, int index) {
    for(int i = 0 ; i < Layer[index].num ; i++)    
        Layer[index].nd[i].weight = setting;
}

void layer_initialization(ld *Layer) {
    for(int i = 0 ; i < LAYERNUM ; i++) {
        Layer[i].nd = (nd*)calloc(sizeof(nd), Layer[i].num);
    }
    weight_initialization(Layer, 1, 0);
}

void layer_free(ld *Layer) {
    for(int i = 0 ; i < LAYERNUM ; i++)    
        free(Layer[i].nd);
}

void node_output(ld *layer) {
    for(int i = 1 ; i < LAYERNUM ; i++) {
        for(int j = 0 ; j < layer[i].num ; j++) {
            for(int k = 0 ; k < layer[i - 1].num ; k++)
                layer[i].nd[j].output += layer[i].nd[j].weight * layer[i - 1].nd[k].output;
        }
        
        for(int j = 0 ; j < layer[i].num ; j++)
            layer[i].nd[j].output = hypertan(layer[i].nd[j].output);
    }
}

void node_set1(ld *layer) {
    // input setting
    layer[0].nd[0].output = 1+(float)65/128;
    layer[0].nd[1].output = 1+(float)15/128;
    layer[0].nd[2].output = 1+(float)80/128;

    // weight setting 1
    layer[1].nd[0].weight = 1+(float)50/128;
    layer[1].nd[1].weight =   (float)10/128;
    layer[1].nd[2].weight =   (float)80/128;
    layer[1].nd[3].weight = 1+(float)10/128;
    layer[1].nd[4].weight = 1+(float)70/128;
    layer[1].nd[5].weight = 1+(float)50/128;
    layer[1].nd[6].weight = 1+(float)10/128;
    layer[2].nd[0].weight = 1+(float)30/128;
    layer[2].nd[1].weight = 1+(float)10/128;
}

int main() {
    ld      layer[3];
    layer[0].num = 3;
    layer[1].num = 7;
    layer[2].num = 2;

    layer_initialization(layer);
    node_set1(layer);

    node_output(layer);     // trigger
    weight_lprint(layer);
    layer_free(layer);

    //weight_lprint(layer);
    //layer_free(layer);
}