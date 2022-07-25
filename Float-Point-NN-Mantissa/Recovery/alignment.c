#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double cov(float *x, float *y, int size)
{
    double  Sxy = 0;
    double  Sx = 0;
    double  Sy = 0;
    int i;
    for(i = 0 ; i < size ; i++)
    {
        Sxy += x[i] * y[i]; // E(XY)
        Sx += x[i];
        Sy += y[i];
    }
    return (Sxy - Sx * Sy / (double)size) / (double)size;
}
void subalign(float *data1, float *data2, int windowsize, int stepsize, int threshold, int TraceLength)
{
    // data 배열에 저장되어 있는 전력파형을 기준으로 data1 배열에 저장되어 있는 전력파형을 정렬
    int     m, j, k;    // 반복문에 쓰일 
    int     size;       // 지금까지 계산된 크기 (windowsize, threshold)
    int     maxcovpos;  // maxcov 값인 상태에서 j의 값(위치)
    float   *x, *y;     // x는 data1의 시작점, y는 data2의 시작점
    double  covval;     // x, y에 대한 cov 값
    double  maxcov;     // 반복문 계산 중 가장 큰 cov 값
    
    // 한번 맞추고 stepsize 만큼 띄워서 정렬을 하면 된다
    // windowsize만큼 threshold를 흔들면서 maxcov를 구한다.
    // (TraceLength - windowsize)를 해준 이유는 끝에 쓸모없는 파형 정리
    for(m = 0 ; m < (TraceLength - windowsize) ; m += stepsize) {
        maxcovpos = 0;
        maxcov = 0;
        for(j = -threshold ; j < threshold ; j++) {
            // 값 설정
            if(j < 0) { // threshold 값이 0보다 작은 경우 -> 정렬할 파형이 뒤에 있을 경우
                x = data1 + m;          // x 시작점은 data + m
                y = data2 + m - j;      // y 시작점은 data + m - j
                size = windowsize + j;  // 배열의 사이즈
            }
            else { // threshold 값이 0보다 큰 경우 -> 정렬할 파형이 앞에 있는 경우
                x = data1 + m + j;      // x 시작점은 data + m + j
                y = data2 + m;          // y 시작점은 data + m
                size = windowsize - j;  // 배열의 사이즈
            }

            // j값 구하기
            covval = cov(x, y, size);   // 원파형과 정렬할 파형의 cov 값 구함
            if(covval > maxcov) {       // 현재까지 계산된 cov보다 클 경우
                maxcovpos = j;          // maxcovpos에 j(위치)를 저장
                maxcov = covval;        // maxcov는 covval로 바꿔준다
            }
        }
        // 정렬 맞추는 코드
        if(maxcovpos < 0) { // 상관계수가 음수이면 정렬할 파형이 뒤에 있는 것 -> 앞으로 밀기
            for(k = m ; k < (TraceLength + maxcovpos) ; k++) {
                data2[k] = data2[k - maxcovpos]; // 정렬할 파형 앞으로 밀기
            }
        }
        else {  // 상관계수가 양수이면 정렬할 파형이 앞에 있는 것 -> 뒤로 밀기
            for(k = (TraceLength - maxcovpos -1) ; k >= m ; k--) {
                data2[k + maxcovpos] = data2[k]; // 정렬할 파형 뒤로 밀기
            }
        }
    }
}
void Alignment(char *arg)
{   // 500 / 450 / 100
    int     windowsize  = 500;  // 부분부분 정렬을 맞추고 싶은 파형의 길이
    int     stepsize    = 300;  // 한 부분의 정렬을 맞춘 후에 몇 포인트씩 이동 후 다시 정렬할 지
    int     threshold   = 200;  // 좌우로 얼마나 흔들면서 cov값을 계산해서 최대값과 이동할 포인트수를 계산
    int     TraceLength, TraceNum;
    
    float   *data;              // 첫 번째 파형 동적할당
    float   *data1;             // 두 번째 파형 동적할당
    
    char    buf[256];
    char    *tok;
    FILE    *RFP, *WFP;
   
    // 읽을 파일
    if((RFP = fopen(arg, "rb")) == NULL) { puts("FILE READ ERR"); return; }
    
    // 쓸 파일
    sprintf(buf, "%s.ali", strtok(arg, "."));
    if((WFP = fopen(buf, "w+b")) == NULL) { puts("FILE WRITE ERR"); return; }

    // 파형 길이와 파형 개수 헤더를 읽고 정렬 파일에 구현
    //fread(&TraceLength, sizeof(int), 1, RFP);
    //fwrite(&TraceLength, sizeof(int), 1, WFP);
    //fread(&TraceNum, sizeof(int), 1, RFP);
    //fwrite(&TraceNum, sizeof(int), 1, WFP);
    // 결과 값 : F01D0100 D0070000
    
    TraceLength = 24000;
    TraceNum = 5000;
    
    data  = (float*)calloc(TraceLength, sizeof(float));
    data1 = (float*)calloc(TraceLength, sizeof(float));
    
    // 첫 번째 파형 로드
    fread(data, sizeof(float), TraceLength, RFP);
    fwrite(data, sizeof(float), TraceLength, WFP);
    
    // 두 번째 파형부터 요리조리 맞추기
    for(int i = 1 ; i < TraceNum ; i++) {
        fread(data1, sizeof(float), TraceLength, RFP);
        subalign(data, data1, windowsize, stepsize, threshold, TraceLength);
        fwrite(data1, sizeof(float), TraceLength, WFP);
        printf("MAIN :: Alignment Processing [%d/%d] (%.2f%%)\r", i, TraceNum, (float)i/TraceNum * 100);
        fflush(stdout);
    }

    fclose(WFP);
    fclose(RFP);
    free(data);
    free(data1);
    printf("Done\n");
}

int main(int argc, char *argv[]) {
    if(argc != 2) { puts("./alignment \"{FileNameNoExtension}\""); return 0; }
    Alignment(argv[1]);
    system("./timing");
}