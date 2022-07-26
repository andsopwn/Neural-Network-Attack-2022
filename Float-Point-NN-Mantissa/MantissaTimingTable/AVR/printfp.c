#include <stdio.h>
void test1() {
    float   res[128];

    for(int i = 0 ; i < 128 ; i++)
        printf("    res[%d] = wt[this] * ip[%d];\n", i, i);
}

void test2() {
    printf("float   weight[100] = {\n");
    for(int i = 0 ; i < 100 ; i++)
        printf("%.2f, ", 1 + (float)i/100);
    printf("};\n");

    printf("float   input[100] = {\n");
    for(int i = 0 ; i < 100 ; i++)
        printf("%.2f, ", 1 + (float)i/100);
    printf("};\n");
}

void test3() {
    printf("float   weight[128] = {\n");
    for(int i = 0 ; i < 128 ; i++)
        printf("%.7f, ", 1 + (float)i/128);
    printf("};\n");

    printf("float   input[128] = {\n");
    for(int i = 0 ; i < 128 ; i++)
        printf("%.7f, ", 1 + (float)i/128);
    printf("};\n");
}

void thismust() {
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x77] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x78] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x79] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x7a] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x7b] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x7c] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x7d] * input[%d];\n", i); puts("");
    //for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x7e] * input[%d];\n", i); puts("");
    for(int i = 0 ; i < 128 ; i++) printf("    result = weight[0x7f] * input[%d];\n", i); puts("");
}

void dataprint() {
    for(int i = 0 ; i < 128 ; i++) {
        printf("    {   // weight = 1.0x%x (%d/128)\n", i, i);
        printf("        \n");
        printf("    },\n");
    }
    puts("");
}

int main() {
    thismust();
    //dataprint();
}