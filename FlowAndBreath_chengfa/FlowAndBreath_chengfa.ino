#include <FastLED.h>

#define Pin1 2

#define LedNum 120
#define N2 32
#define DelayTime 30

CRGB L1[LedNum];
//float F[LedNum] = { 0 };


void setup() {
  Serial.begin(9600);

  pinMode(Pin1, OUTPUT);

  // for (uint8_t j = 0; j < LedNum; j++) {
  //   F[j] = 0.2*sin(j / 2) * sin(j / 2)+0.8;
  // }

  LEDS.addLeds<WS2812, Pin1, GRB>(L1, LedNum);
}

//uint32_t j = 0;
uint8_t i, j, k = 0;

void loop() {
  for (i = 0; i < N2; i++) {
    for (j = 0; j < LedNum; j++) {
      //C_Red = (int)(i * i * 0.26 * F[(k + j) % LedNum]);
      L1[j] =CRGB((int)(i * i * 0.26 * 255), 0, 0);
    }
    FastLED.show();
    delay(DelayTime);
    k++;
  }
  ///////////
  for (i = 0; i < N2; i++) {
    for (j = 0; j < LedNum; j++) {
      //C_Red = 255 - i * 8;
      //C_Red = (int)((255.0 - i * i * 0.26) * F[(k + j) % LedNum]);
      L1[j] = CRGB((int)((255.0 - i * i * 0.26*255) ), 0, 0);
      //C_Red = (int)(255 - i * 8) * F[(k + j) % LedNum];
      //C_Red = (int)(255 ) * F[(k + j) % LedNum];
    }
    FastLED.show();
    delay(DelayTime);
    k++;
  }
  ///////////
  for (i = 0; i < N2; i++) {
    for (j = 0; j < LedNum; j++) {
      //C_Blu = i * 8;
      //C_Blu = (int)(i * i * 0.26 * F[(k + j) % LedNum]);
      L1[j] = CRGB(0, 0, (int)(i * i * 0.26 *255));
      //C_Blu = (int)i * 8 * F[(k + j) % LedNum];
      //C_Blu = (int)255* F[(k + j) % LedNum];
    }
    FastLED.show();
    delay(DelayTime);
    k++;
  }
  ///////////
  for (i = 0; i < N2; i++) {
    for (j = 0; j < LedNum; j++) {
      //C_Blu = 255 - i * 8;
      //C_Blu = (int)((255.0 - i * i * 0.26) * F[(k + j) % LedNum]);
      L1[j] = CRGB(0, 0, (int)((255.0 - i * i * 0.26*255) ));
      //C_Blu = (int)(255 - i * 8) * F[(k + j) % LedNum];
      //C_Blu = (int)(255) * F[(k + j) % LedNum];
    }
    FastLED.show();
    delay(DelayTime);
    k++;
  }
}

