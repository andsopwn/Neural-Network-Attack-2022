#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef union {
   float f;
   unsigned long long y;
   struct {
      unsigned int mantissa : 23;
      unsigned int exponent : 8;
      unsigned int sign : 1;
   }a;
}data;
data input;
data weight;
data last;
data test;
void CPA();
void CPA_Mantissa();
void CPA_Exponent();
void CPA_Sign();
int count = 0;



void CPA_Mantissa(char *InputfilePath, char *InputfileFormat, char *OutputfilePath, float **plaintext,
   int TraceLength, int TraceNum, int targetbit, int setbit, int WeightPos, int startpoint, int endpoint) {

   double *corrT = NULL;
   double max, lastmax, test1, test2;
   short *data = NULL;
   double *Sxx = NULL, *Sx = NULL;
   unsigned int output, temp1=0;
   double *Sxy = NULL;
   unsigned char hw_iv;
   char buf[256];
   
   int err = 0, j, k, x, key, maxkey, maxpoint, w_pos, index, Sy, Syy;

   FILE* rfp, *wfp;

   // 데이터 공간 할당
   Sx = (double*)calloc(TraceLength, sizeof(double));
   data = (double*)calloc(TraceLength, sizeof(double));
   Sxx = (double*)calloc(TraceLength, sizeof(double));
   Sxy = (double*)calloc(TraceLength, sizeof(double));
   corrT = (double*)calloc(TraceLength, sizeof(double));

   weight.a.mantissa = 0;

   //weight mantissa(23bit)를 3bit씩 쪼개서 복원, CPA 총 8회 반복
   //최상위 7bit 추측 -> weight mantissa 최상위 3bit 복원 -> weight mantissa 그 다음 상위 3bit 복원

   w_pos = WeightPos - 1;
   for (int m = 0; m < 24 / targetbit; m++) {
      max = 0;
      maxpoint = 0;
      maxkey = 0;

      for (unsigned int key = 0; key < setbit; key++) {
         Sy = 0;
         Syy = 0;

         memset(Sxy, 0, sizeof(double) * TraceLength);
         memset(corrT, 0, sizeof(double) * TraceLength);

         for (int i = 0; i < TraceNum; i++) {
            memset(data, 0, sizeof(short)*TraceLength);

            sprintf_s(buf, 256 * sizeof(char)/*sizeof(buf)*/, "%s%s%05d.trc", InputfilePath, InputfileFormat, i);
            if ((err = fopen_s(&rfp, buf, "rb"))) {
               printf("File Open Error1!!\n");

            }
            fseek(rfp, 357, SEEK_SET); // trc 파일 헤더 건너뛰기

            fread(data, sizeof(short), TraceLength, rfp); // trc 파일 읽기

            if ((key == 0) && (m == 0))
            {
               for (int j = startpoint; j < endpoint; j++) {
                  Sx[j] += (double)data[j];
                  Sxx[j] += (double)data[j] * data[j];
               }
            }

            input.f = plaintext[i][w_pos]; //알고 있는 입력값 불러오기

            x = input.a.mantissa; // 입력값의 mantissa bit 저장
            x += 0x800000;

            //1.xmantissa * 1.wmantissa 에서 1.~부분을 x의 값에 더해주는 역할
            output = (key + 128) * x; // 결과는 binary로 1000 0000 0000 0000 0000 0000 의미

            hw_iv = 0;
            if (m == 0) {
               output = output & (0x7f000000); // 최상위 7bit만 가져오기
               for (int i = 24; i < 31; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 1) {
               output = output & (0x0fe00000); // 7bit만 가져오기
               for (int i = 21; i < 28; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 2) {
               output = output & (0x01fc0000);
               for (int i = 18; i < 25; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 3) {
               output = output & (0x003f8000);
               for (int i = 15; i < 22; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 4) {
               output = output & (0x0007f000);
               for (int i = 12; i < 19; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 5) {
               output = output & (0x0000fe00);
               for (int i = 9; i < 16; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 6) {
               output = output & (0x00001f00);
               for (int i = 6; i < 13; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            else if (m == 7) {
               output = output & (0x000003f0);
               for (int i = 3; i < 10; i++) {
                  hw_iv += (output >> i) & 0x1;

               }
            }
            /*
            index = 0;
            for (int i = 0; i < 32; i++) {
            if (((output >> (31 - i)) & 1) == 1) {
            index = i + 1;
            break;
            }
            }
            //hw 설정
            hw_iv = 0;
            for (k = 23; k < 32; k++) {
            hw_iv += (output >> (31 - (index + k)) & 1);
            printf("%d", (output >> (31 - (index + k)) & 1));
            }*/

            Sy += hw_iv;
            Syy += hw_iv * hw_iv;
            for (int k = startpoint; k < endpoint; k++) {
               Sxy[k] += (double)(hw_iv * data[k]);
            }

            fclose(rfp);
         }


         for (k = startpoint; k < endpoint; k++) {
            test1 = (double)(TraceNum * Sxy[k]) - (double)(Sx[k] * Sy);
            test2 = sqrt(((double)(TraceNum * Sxx[k] - Sx[k] * Sx[k]) * (double)(TraceNum * Syy - Sy * Sy)));
            corrT[k] = test1 / test2;
            if (fabs(corrT[k]) > max) {
               maxkey = key;
               maxpoint = k;
               max = fabs(corrT[k]);
            }
         }
         //printf(".");
      }


      snprintf(buf, 256 * sizeof(char), "%s/WP%04d-MantissaCorr-M%02d-MaxKey%04d.bin", OutputfilePath, WeightPos, m, maxkey);
      if ((err = fopen_s(&wfp, buf, "wb"))) {
         printf("File Open Error3!!\n");
      }

      fwrite(corrT, sizeof(double), TraceLength, wfp);
      fclose(wfp);

      printf("WP%04d-%dth weight mantissa : maxpoint(%d), maxkey(0x%02x), maxbit(0x%02x), maxcorr(%lf)\n",
         WeightPos, m, maxpoint, maxkey, maxkey & (0x70), max);

      // 7bit 씩 추측일 경우 mantissa에 구한 비트 값 더해서 설정하기


      if ((setbit == 0x7f) && (m == 0)) {
         temp1 += ((maxkey & 0x70)/0x10) * 0x100000; // 최상위 3비트
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 1)) {
         temp1 += ((maxkey & 0x70) / 0x10) * 0x20000;
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 2)) {
         temp1 += ((maxkey & 0x70) / 0x10) * 0x4000;
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 3)) {
         temp1 += ((maxkey & 0x70) / 0x10) * 0x800;
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 4)) {
         temp1 += ((maxkey & 0x70) / 0x10) * 0x100;
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 5)) {
         temp1 += ((maxkey & 0x70) / 0x10) * 0x20;
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 6)) {
         temp1 += ((maxkey & 0x70) / 0x10) * 0x4;
         //printf("temp1 : %x\n", temp1);
      }
      else if ((setbit == 0x7f) && (m == 7)) {
         temp1 += ((maxkey & 0x60) / 0x20);
         //printf("temp1 : %x\n", temp1);
      }
   }

   
   weight.a.mantissa = temp1;
   //printf("temp1 : %x\n", temp1);
   printf("weight mantissa : %x\n", weight.a.mantissa);


   free(Sx);
   free(Sxx);
   free(data);
   free(Sxy);
   free(corrT);



}
void CPA_Exponent(char *InputfilePath, char *InputfileFormat, char *OutputfilePath, float **plaintext,
   int TraceLength, int TraceNum, int WeightPos, int startpoint, int endpoint) {

   double *corrT = NULL;
   double max, lastmax, test1, test2;
   short *data = NULL;
   double *Sxx = NULL, *Sx = NULL;
   unsigned int output;
   double *Sxy = NULL;
   unsigned char hw_iv;
   char buf[256];
   int err = 0, j, k, x, key, maxkey, maxpoint, w_pos, index, Sy, Syy;
   FILE* rfp, *wfp;

   // 데이터 공간 할당
   Sx = (double*)calloc(TraceLength, sizeof(double));
   data = (double*)calloc(TraceLength, sizeof(double));
   Sxx = (double*)calloc(TraceLength, sizeof(double));
   Sxy = (double*)calloc(TraceLength, sizeof(double));
   corrT = (double*)calloc(TraceLength, sizeof(double));


   unsigned int w_zero_count = 0; // w의 끝 0의 개수
   unsigned long long w = weight.a.mantissa;
   for (int i = 0; i < 23; i++) {
      if (((w >> i) & 1) == 0) {
         w_zero_count += 1;
      }
      if (((w >> i) & 1) == 1) {
         break;
      }
   }// weight mantissa 모두 구했다고 가정
   
    // weight의 exponent가 key 
   w_pos = WeightPos - 1;
   w += 0x80000;

   max = 0;
   maxpoint = 0;
   maxkey = 0;

   for (unsigned int key = 1; key < 256; key++) {

      Sy = 0;
      Syy = 0;
      memset(Sxy, 0, sizeof(double) * TraceLength);
      memset(corrT, 0, sizeof(double) * TraceLength);
      weight.a.exponent = key;

      for (int i = 1; i < TraceNum; i++) {

         memset(data, 0, sizeof(short)*TraceLength);

         sprintf_s(buf, 256 * sizeof(char)/*sizeof(buf)*/, "%s%s%05d.trc", InputfilePath, InputfileFormat, i);
         if ((err = fopen_s(&rfp, buf, "rb"))) {
            printf("File Open Error1!!\n");
         }
         fseek(rfp, 357, SEEK_SET); // trc 파일 헤더 건너뛰기

         fread(data, sizeof(short), TraceLength, rfp); // trc 파일 읽기

         if (key == 1) {
            for (int j = startpoint; j < endpoint; j++) {
               Sx[j] += (double)data[j];
               Sxx[j] += (double)(data[j] * data[j]);
            }
         }

         input.f = plaintext[i][w_pos]; // 노드의 입력값 저장
         last.f = input.f * weight.f; // 입력값과 가중치의 곱

         hw_iv = 0;

         for (int i = 0; i < 32; i++) {
            hw_iv += ((int)last.f >> i) & 0x01; //입력값과 가중치의 곱을 1비트 씩 뽑아 hw_iv 변수에 저장
            //printf("%d", (test.y >> i) & 0x01);
         }

         Sy += hw_iv;
         Syy += hw_iv * hw_iv;
         for (int k = startpoint; k < endpoint; k++) {
            Sxy[k] += (double)(hw_iv * data[k]);
         }

         fclose(rfp);

      }
      for (k = startpoint; k < endpoint; k++) {
         corrT[k] = ((double)TraceNum * Sxy[k] - Sx[k] * Sy) / sqrt(((double)TraceNum * Sxx[k] - Sx[k] * Sx[k]) * ((double)TraceNum * Syy - Sy * Sy));
         if (fabs(corrT[k]) > max) {
            maxkey = key;
            maxpoint = k;
            max = fabs(corrT[k]);
         }
      }
   }


   snprintf(buf, 256 * sizeof(char), "%s/WP%04d-TR%05d-ExponentCorr-MaxKey%04d.bin", OutputfilePath, WeightPos, TraceNum, maxkey);
   wfp = fopen(buf, "wb");
   if (wfp == NULL) {
      printf( "Cannot open %s file !", buf);
   }
   fwrite(corrT, sizeof(double), TraceLength, wfp);
   fclose(wfp);

   printf("WP%04d-TR%05d's weight exponent : maxpoint(%d), maxkey(%02x), maxcorr(%lf)\n", WeightPos, TraceNum, maxpoint, maxkey, max);
   weight.a.exponent = maxkey; // weight의 exponent를 maxkey라고 설정

   printf("weight exponent : %d\n", weight.a.exponent);


   free(Sx);
   free(Sxx);
   free(data);
   free(Sxy);
   free(corrT);



}
void CPA_Sign(char *InputfilePath, char *InputfileFormat, char *OutputfilePath, float **plaintext,
    int TraceLength, int TraceNum, int WeightPos, int startpoint, int endpoint) {

   double *corrT = NULL;
   double max, lastmax, test1, test2, plus, minus;
   short *data = NULL;
   double *Sxx = NULL, *Sx = NULL;
   unsigned int output;
   double *Sxy = NULL;
   unsigned char hw_iv;
   char buf[256];
   int err = 0, j, k, x, key, maxkey, maxpoint, w_pos, index, Sy, Syy;
   FILE* rfp, *wfp;

   // 데이터 공간 할당
   Sx = (double*)calloc(TraceLength, sizeof(double));
   data = (double*)calloc(TraceLength, sizeof(double));
   Sxx = (double*)calloc(TraceLength, sizeof(double));
   Sxy = (double*)calloc(TraceLength, sizeof(double));
   corrT = (double*)calloc(TraceLength, sizeof(double));

   w_pos = WeightPos - 1;

   max = 0;
   maxpoint = 0;

   Sy = 0;
   Syy = 0;
   memset(Sxy, 0, sizeof(double) * TraceLength);
   memset(corrT, 0, sizeof(double) * TraceLength);

   /**********weight의 부호가 음수일 때 상관계수 계산***********/
   weight.a.sign = 1;

   for (int i = 0; i < TraceNum; i++) {
      memset(data, 0, sizeof(short)*TraceLength);

      sprintf_s(buf, 256 * sizeof(char)/*sizeof(buf)*/, "%s%s%05d.trc", InputfilePath, InputfileFormat, i);
      if ((err = fopen_s(&rfp, buf, "rb"))) {
         printf("File Open Error1!!\n");
      }
      fseek(rfp, 357, SEEK_SET); // trc 파일 헤더 건너뛰기

      fread(data, sizeof(short), TraceLength, rfp); // trc 파일 읽기

      for (int j = startpoint; j < endpoint; j++) {
         Sx[j] += (double)data[j];
         Sxx[j] += (double)data[j] * data[j];
      }

      input.f = plaintext[i][w_pos] * weight.f;
      hw_iv = 0;

      for (int s = 0; s < 31; s++) {
         hw_iv += ((unsigned int)input.f >> s) & 0x1;

      }

      hw_iv -= 1;
      Sy += hw_iv;
      Syy += hw_iv * hw_iv;
      for (int k = startpoint; k < endpoint; k++) {
         Sxy[k] += (double)hw_iv * data[k];
      }

      fclose(rfp);
   }

   for (k = startpoint; k < endpoint; k++) {
      corrT[k] = ((double)TraceNum * Sxy[k] - Sx[k] * Sy) / sqrt(((double)TraceNum * Sxx[k] - Sx[k] * Sx[k]) * ((double)TraceNum * Syy - Sy * Sy));
      if (fabs(corrT[k]) > max) {
         maxpoint = k;
         max = fabs(corrT[k]);
      }
   }

   minus = fabs(corrT[maxpoint]); // maxpoint에서의 상관계수를 저장
   //printf("minus : %lf\n", minus);

   /**********weight의 부호가 양수일 때 상관계수 계산*********/
   weight.a.sign = 0;

   max = 0;
   maxpoint = 0;

   Sy = 0;
   Syy = 0;

   memset(Sxy, 0, sizeof(double) * TraceLength);
   memset(corrT, 0, sizeof(double) * TraceLength);
   memset(Sx, 0, sizeof(double) * TraceLength);
   memset(Sxx, 0, sizeof(double) * TraceLength);

   for (int i = 0; i < TraceNum; i++) {
      memset(data, 0, sizeof(short)*TraceLength);

      sprintf_s(buf, 256 * sizeof(char)/*sizeof(buf)*/, "%s%s%05d.trc", InputfilePath, InputfileFormat, i);
      if ((err = fopen_s(&rfp, buf, "rb"))) {
         printf("File Open Error1!!\n");
      }
      fseek(rfp, 357, SEEK_SET); // trc 파일 헤더 건너뛰기

      fread(data, sizeof(short), TraceLength, rfp); // trc 파일 읽기

      for (int j = startpoint; j < endpoint; j++) {
         Sx[j] += (double)data[j];
         Sxx[j] += (double)data[j] * data[j];
      }

      input.f = plaintext[i][w_pos] * weight.f;
      hw_iv = 0;

      for (int s = 0; s < 31; s++) {
         hw_iv += ((unsigned int)input.f >> s) & 0x1;

      }

      Sy += hw_iv;
      Syy += hw_iv * hw_iv;

      for (int k = startpoint; k < endpoint; k++) {
         Sxy[k] += (double)hw_iv * data[k];
      }

      fclose(rfp);
   }
   for (int k = startpoint; k < endpoint; k++) {
      corrT[k] = ((double)TraceNum * Sxy[k] - Sx[k] * Sy) / sqrt(((double)TraceNum * Sxx[k] - Sx[k] * Sx[k]) * ((double)TraceNum * Syy - Sy * Sy));
      if (fabs(corrT[k]) > max) {
         maxpoint = k;
         max = fabs(corrT[k]);
      }
   }

   plus = fabs(corrT[maxpoint]); // maxpoint에서의 상관계수를 저장
   //printf("plus : %lf\n", plus);

   /*****************부호 판정 부분*******************/
   if (plus > minus) { // 부호가 양수일 때 상관계수가 더 크면 weight의 부호를 양수로 판정

      weight.a.sign = 0;

      snprintf(buf, 256 * sizeof(char), "%s/WP%04d-TR%05d-SignCorr-MaxKey%04d.bin", OutputfilePath, WeightPos, TraceNum, 0);
      wfp = fopen(buf, "wb");
      if (wfp == NULL) {
         printf( "Cannot open %s file !", buf);
      }
      fwrite(corrT, sizeof(double), TraceLength, wfp);
      fclose(wfp);

      printf( "WP%04d-TR%05d's weight sign : maxpoint(%d), maxcorr(%lf), keymaxcorr(%lf)\n",
         WeightPos, TraceNum, maxpoint, max, plus);
   }
   else if (plus < minus) { // 부호가 음수일 때 상관계수가 더 크면 weight의 부호를 음수로 판정

      weight.a.sign = 1;

      snprintf(buf, 256 * sizeof(char), "%s/WP%04d-TR%05d-SignCorr-MaxKey%04d.bin", OutputfilePath, WeightPos, TraceNum, 1);
      wfp = fopen(buf, "wb");
      if (wfp == NULL) {
         printf( "Cannot open %s file !", buf);
      }
      fwrite(corrT, sizeof(double), TraceLength, wfp);
      fclose(wfp);

      printf( "WP%04d-TR%05d's weight sign bit: maxpoint(%d), maxcorr(%lf), keymaxcorr(%lf)\n",
         WeightPos, TraceNum, maxpoint, max, minus);
   }

   printf("weight signbit : %d\n", weight.a.sign);

   free(Sx);
   free(Sxx);
   free(data);
   free(Sxy);
   free(corrT);


}

// InputfilePath : 입력 파일 경로, InputfileFormat : 입력 파일 포맷, OutputfilePath : 출력 파일 경로, OutputfileFormat : 출력 파일 포맷
// NodeInputFile : 입력 실수 값이 저장되어 있는 파일, TraceLength : 파형 길이, TraceNum : 파형 개수, NodeNum : 입력 노드 개수, WeightPos : 분석 대상 weight
// StartPoint : 분석 시작점,  EndPoint : 분석 끝점 

void CPA(char *InputfilePath, char *InputfileFormat, char *OutputfilePath, char *OutputfileFormat, char *NodeInputFile,
   int TraceLength, int TraceNum, int NodeNum, int WeightPos, int StartPoint, int EndPoint) {

   float** plaintext = NULL;
   unsigned char temp[34], x, y, iv, hw_iv;
   char buf[256];
   int err = 0, i, j, k, key, maxkey, maxpoint, targetbit, setbit;

   FILE* rfp = NULL, *wfp = NULL;


   // 입력 노드에 들어가는 실수값이 저장되어 있는 파일 불러오기
   plaintext = (float**)calloc(TraceNum, sizeof(float*));
   sprintf_s(buf, 256 * sizeof(char), "%s", NodeInputFile);
   if ((err = fopen_s(&rfp, buf, "rb"))) {
      printf("File Open Error2!!\n");
   }
   for (i = 0; i < TraceNum; i++) {
      plaintext[i] = (float*)calloc(4, sizeof(float)); //4byte 씩 읽어들임 (4는 고정값
   }
   for (i = 0; i < TraceNum; i++) {
      fread(plaintext[i], sizeof(float), 4, rfp);
   }
   fclose(rfp);


   setbit = 0x7f; // mantissa 추측 비트 사용자 설정 (0x7f -> 7bit)
   targetbit = 3; //mantissa 출력 비트 사용자 설정 (3bit 뽑기)
   WeightPos = 1;


   CPA_Mantissa(InputfilePath, InputfileFormat, OutputfilePath, plaintext, TraceLength, TraceNum, targetbit, setbit, WeightPos, StartPoint, EndPoint); // Mantissa 추측 함수 불러오기
   CPA_Exponent(InputfilePath, InputfileFormat, OutputfilePath, plaintext, TraceLength, TraceNum, WeightPos, StartPoint, EndPoint); // Exponent 추측 함수 불러오기
   CPA_Sign(InputfilePath, InputfileFormat, OutputfilePath, plaintext, TraceLength, TraceNum, WeightPos, StartPoint, EndPoint); // Sign 추측 함수 불러오기

   printf("%lf\n", weight.f); // 복원한 weight 값 출력

   free(plaintext);


}
int main() {


   char InputfilePath[256] = "C:\Users\master\Desktop\mlp\align";
   char InputfileFormat[256] = "Z2--mlp--align--";
   char OutputfilePath[256] = "C:\Users\master\Desktop\mlp\align";
   char OutputfileFormat[256] = "trc";
   char NodeInputFile[256] = "C:\\Users\\master\\Downloads\\sujin-20211129T062419Z-001\\sujin\\align\\textin10000.npy";
   int TraceLength = 56022;
   int TraceNum = 10000;
   int NodeNum = 3;
   int StartPoint = 6000;
   int EndPoint = 22000;
   float WeightPos = 0;

   CPA(InputfilePath, InputfileFormat, OutputfilePath, OutputfileFormat, NodeInputFile, TraceLength, TraceNum, NodeNum, WeightPos, StartPoint, EndPoint);



   return 0;
}