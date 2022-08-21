#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef union {
  float num;
  struct {
    unsigned int m : 23;
    unsigned int e : 8;
    unsigned int s : 1;
  } p;
} f;
float   sigmoid(float h)    { return (1 / (1 + exp((-1)*h))); }
float   hypertan(float h)   { return (2 / (1 + exp(-2 * h)) - 1); }
float   relu(float h)       { return (h > 0 ? h : 0);     }

int main() {
    float   ma[5] = { 1.0391, 1.6641, 1.0859, 1.1797, 1.1250 }; // 133, 213, 139, 151, 144
    float   pa;
    int     ipk1;
    int     fg;
    int     loc;
    f   wt[5], b;
    wt[0].num = 0.25975;
    wt[1].num = -0.208775;
    wt[2].num = -0.0169609375;
    wt[3].num = 0.295075;
    wt[4].num = 0.00879296875;
    b.num = -50.8992;
    
    // first zero-cross
    fg = 1;
    loc = 0;
    for(int i = 0 ; i < 256 ; i++) {
        pa = wt[loc].num * (float)i + b.num;
        printf("ZERO-CROSS.IP 1st[%d]\t ACTUAL[%f]\n", i, pa);
        if((pa >= 0) && (fg == 1)) { ipk1 = i; fg = 10; }
    }
    if(fg == 10) {
        printf("\n\tZC IP 1st = %d\n", ipk1);


    }
    // second zero-cross
    else { 
        printf("\n\tZC IP 1st FAIL!\n");
    for(int i = 0 ; i < 256 ; i++) {
        pa = ((float)i * wt[loc].num) + b.num - (wt[loc].num * (float)255);
        printf("ZERO-CROSS.IP 2st[%d]\t ACTUAL[%f]\n", i, pa);
        if((pa >= 0) && (fg == 1)) { ipk1 = i; fg = 10; }
    }

    }
}