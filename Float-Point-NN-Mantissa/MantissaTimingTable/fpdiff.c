#include <stdio.h>
#include "data.h"
#define num 130

void fixedinput() {
    int loc[num] = { 
 };
    puts("DIFF");
    for(int i = 0 ; i < num ; i++) {
        if(loc[i+1] == 0) return;
        printf("%d, ", loc[i+1] - loc[i]);

    }
}

void flexinput() {
    int timing[128];
    for(int i = 0 ; i < 127 ; i++)
        scanf(" %d ", &timing[i]);
    puts("");
    for(int i = 0 ; i < 126 ; i++) {
        //if(i == 1) printf("164, ");
        printf("%d, ", timing[i+1] - timing[i]);
    }

    printf("152");
    puts("");
}

void whatdiff() {
    int same = 1;
    for(int i = 0 ; i < 127 ; i++) {
        same = 1;
        for(int k = 0 ; k < 128 ; k++) {
            if(timing[i][k] != timing[i+1][k])
            same = 0;
        }
        if(same == 1)   printf("0x%02X[%03d] = 0x%02X[%03d]\n", i, i+1, i, i+1);
    }
}
int main() {
    //flexinput();
    whatdiff();
}