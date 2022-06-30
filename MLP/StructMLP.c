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
    // weight setting 1
    layer[1].nd[0].weight = -1.03;
    layer[1].nd[1].weight =  0.62;
    layer[1].nd[2].weight = -0.51;
    layer[1].nd[3].weight = -0.31;
    layer[1].nd[4].weight =  0.29;
    layer[1].nd[5].weight = -0.45;
    layer[2].nd[0].weight = -0.40;
    layer[2].nd[1].weight =  0.84;
}

int main() {
    ld      layer[3];
    layer[0].num = 3;
    layer[1].num = 6;
    layer[2].num = 2;

    layer_initialization(layer);
    node_set1(layer);
    
    layer[0].nd[0].output = data[10];
    layer[0].nd[1].output = data[11];
    layer[0].nd[2].output = data[12];
    node_output(layer);     // trigger
    weight_lprint(layer);
    layer_free(layer);

    //weight_lprint(layer);
    //layer_free(layer);
}