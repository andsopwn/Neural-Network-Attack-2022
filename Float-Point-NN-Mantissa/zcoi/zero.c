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
float   ma[5] = { 1.0391, 1.6641, 1.0859, 1.1797, 1.1250 }; // 133, 213, 139, 151, 144
float   wt[5] = { 0.25975, -0.208775, -0.0169609375, 0.295075, 0.00879296875 };
int ipk3(const float wt) {
    float   pa;
    float   ip;
    for(int i = 0 ; i < 256 ; i++) {
        pa = (wt * 255) + ((float)i * 0.25975) + bias;
        //printf("ZERO-CROSS.IP 3rd[%d]\t PA[%f]\n", i, pa);
        if((pa >= 0)) { return  i; }
    }
    return -1;
}
int ipk2(const float wt) {
    float   pa;
    float   ip;
    for(int i = 0 ; i < 256 ; i++) {
        pa = (wt * (float)i) + (255 * 0.25975) + bias;
        //printf("ZERO-CROSS.IP 2nd[%d]\t PA[%f]\n", i, pa);
        if((pa <= 0)) { return  i; }
    }
    return -1;
}
int ipk1(const float wt) {
    float   pa;
    float   ip;
    for(int i = 0 ; i < 256 ; i++) {
        pa = wt * (float)i + bias;
        //printf("ZERO-CROSS.IP 1st[%d]\t PA[%f]\n", i, pa);
        if((pa >= 0)) { return  i; }
    }
    return -1;
}

void exploit(const float target, const int ipk, const int round) {
    puts("\n\t::ZERO-CROSSOVER Input ::");
    printf("IPK Extract Round \t%d\n", round);
    printf("IPK Value \t\t%d\n", ipk);

}

void zerocross() {
    float   target = wt[0];
    int     val  = ipk1(target);
    if(val == -1 || val == 0)    puts("ZERO-CROSS. IP 1st Round Failed!");
    else {
        printf("ZERO-CROSS. IP 1st Round[%d]\n", val);
        exploit(target, val, 1);
        return;
    }
    val = ipk2(target);
    if(val == -1 || val == 0)   puts("ZERO-CROSS. IP 2nd Round Failed!");
    else {
        printf("ZERO-CROSS. IP 2nd Round[%d]\n", val);
        exploit(target, val, 2);
        return;
    }
    val = ipk3(target);
    if(val == -1 || val == 0)   puts("ZERO-CROSS. IP 3rd Round Failed!");
    else {
        printf("ZERO-CROSS. IP 3rd Round[%d]\n", val);
        exploit(target, val, 3);
        return;
    }
}

int main() {
    zerocross();
}
