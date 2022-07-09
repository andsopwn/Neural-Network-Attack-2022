#include <stdio.h>
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
int main() {
    flexinput();
}