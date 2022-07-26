#include <stdio.h>
#define num 10000
int main() {
    float   ap[num];
    float   ip = 1.0;
    float   wt = 1.0;

    for(int i = 0 ; i < num ; i++) {
        ip = 1.0;
        for(int k = 0 ; k < num ; k++, ip+=(float)(1/num) ) {
            //fflush(stdout);
            ap[k] = ip * wt;
            
        }
        wt += (float)(1 / num);
    }
    puts("");
}