#include <stdio.h>
#include <x86intrin.h>

int main(void) {
    int cpuInfo[4] = { 0, 0, 0, 0 };
    __cpuid(cpuInfo, 0);
    if (cpuInfo[0] >= 0x16) {
        __cpuid(cpuInfo, 0x16);

        //Example 1
        //Intel Core i7-6700K Skylake-H/S Family 6 model 94 (506E3)
        //cpuInfo[0] = 0x00000FA0; //= 4000 MHz
        //cpuInfo[1] = 0x00001068; //= 4200 MHz
        //cpuInfo[2] = 0x00000064; //=  100 MHz

        //Example 2
        //Intel Core m3-6Y30 Skylake-U/Y Family 6 model 78 (406E3)
        //cpuInfo[0] = 0x000005DC; //= 1500 MHz
        //cpuInfo[1] = 0x00000898; //= 2200 MHz
        //cpuInfo[2] = 0x00000064; //=  100 MHz

        //Example 3
        //Intel Core i5-7200 Kabylake-U/Y Family 6 model 142 (806E9)
        //cpuInfo[0] = 0x00000A8C; //= 2700 MHz
        //cpuInfo[1] = 0x00000C1C; //= 3100 MHz
        //cpuInfo[2] = 0x00000064; //=  100 MHz

        printf("EAX: 0x%08x EBX: 0x%08x ECX: %08x\r\n", cpuInfo[0], cpuInfo[1], cpuInfo[2]);
        printf("Processor Base Frequency:  %04d MHz\r\n", cpuInfo[0]);
        printf("Maximum Frequency:         %04d MHz\r\n", cpuInfo[1]);
        printf("Bus (Reference) Frequency: %04d MHz\r\n", cpuInfo[2]);
    } else {
        printf("CPUID level 16h unsupported\r\n");
    }
}