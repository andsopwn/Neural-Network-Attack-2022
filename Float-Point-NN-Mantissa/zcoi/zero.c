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

int rr(const float a) { // -5.4
    int     con = (int)a;
    float   cal = a;
    float     ret = a;
    cal -= con;

    if(cal < 0) {
        if(cal >= -0.5) return (int)ret;
        else            return (int)ret - 1;
    } else {
        if(cal >= 0.5)  return (int)ret + 1;
        else            return (int)ret;
    }

    return 0;
}
float   bias  = -50.8992;

int ipk3(const float wt) {
    float   pa;
    float   ip;
    for(int i = 0 ; i < 256 ; i++) {
        pa = (wt * 255) + ((float)i * 0.25975) + bias;
        //printf("ZERO-CROSS.IP 2nd[%d]\t ACTUAL[%f]\n", i, pa);
        if((pa >= 0)) { return  i; }
    }
    return -1;
}
int ipk2(const float wt) {
    float   pa;
    float   ip;
    for(int i = 0 ; i < 256 ; i++) {
        pa = (wt * (float)i) + (255 * 0.25975) + bias;
        //printf("ZERO-CROSS.IP 2nd[%d]\t ACTUAL[%f]\n", i, pa);
        if((pa <= 0)) { return  i; }
    }
    return -1;
}
int ipk1(const float wt) {
    float   pa;
    float   ip;
    for(int i = 0 ; i < 256 ; i++) {
        pa = wt * (float)i + bias;
        //printf("ZERO-CROSS.IP 1st[%d]\t ACTUAL[%f]\n", i, pa);
        if((pa >= 0)) { return  i; }
    }
    return -1;
}

void zerocross() {
    float   ma[5] = { 1.0391, 1.6641, 1.0859, 1.1797, 1.1250 }; // 133, 213, 139, 151, 144
    float   wt[5] = { 0.25975, -0.208775, -0.0169609375, 0.295075, 0.00879296875 };

    printf("%d\n", ipk1(wt[1]));
    printf("%d\n", ipk2(wt[1]));
    printf("%d\n", ipk3(wt[1]));
}

int main() {
    zerocross();

}
