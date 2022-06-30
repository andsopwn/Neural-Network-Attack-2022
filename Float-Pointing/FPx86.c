#include <stdio.h>
#define sv 10000
int main() {
    float   ap[sv];
    float   ip = 1.0;
    float   wt = 1.0;

    for(int i = 0 ; i < sv ; i++) {
        ip = 1.0;
        for(int k = 0 ; k < sv ; k++, ip+=(float)(1/sv) ) {
            //fflush(stdout);
            ap[k] = ip * wt;
            
        }
        wt += (float)(1 / sv);
    }
    puts("");
}