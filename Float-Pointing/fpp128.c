#include <stdio.h>
#define     jump    128

float   wt[jump];
float   ip[jump];

int main() {
    float   unit = (float)1/(float)128;
	
	for(int i = 0 ; i < jump ; i++) {
		wt[i] = 1.0 + (float)unit * i;
		ip[i] = 1.0 + (float)unit * i;
	}

    for(int i = 0 ; i < jump ; i++) 
    printf("%f %f %f\n", wt[i], ip[i] , wt[i] * ip[i]);

    
}