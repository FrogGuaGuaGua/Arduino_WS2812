#include <FastLED.h>

#define Pin1 13
#define Pin2 2
#define Pin20 3
#define Pin3 4
#define Pin30 5
#define Pin4 6
#define Pin40 7
#define Pin4_ 8
#define Pin5 9
#define Pin50 10
#define Pin6 11
#define Pin60 12


const uint8_t N1 = 56, N2 = 34, N20 = 29, N3 = 19, N30 = 19, N4 = 24, N40 = 21, N4_ = 36;
// const uint8_t N5 = 48, N50 = 50, N6 = 19, N60 = 24;
const uint8_t N5 = 50, N50 = 50, N6 = 19, N60 = 24;

CRGB L1[N1], L2[N2], L20[N20], L3[N3], L30[N30], L4[N4], L40[N40], L4_[N4_], L5[N5], L50[N50], L6[N6], L60[N60];  //

uint8_t F1[2 * N1] = { 0 }, F2[2 * N2] = { 0 }, F20[2 * N20] = { 0 }, F3[2 * N3] = { 0 }, F30[2 * N30] = { 0 };
uint8_t F4[2 * N4] = { 0 }, F40[2 * N40] = { 0 }, F4_[2 * N4_] = { 0 }, F5[2 * N5] = { 0 }, F50[2 * N50] = { 0 };
uint8_t F6[2 * N6] = { 0 }, F60[2 * N60] = { 0 };

void assign(uint8_t* p, uint8_t N) {
  double tmp;
  for (uint8_t i = 0; i < N / 2; i++) {
    tmp = 255.0 / ((N / 2) * (N / 2)) * i * i;
    p[i] = tmp;
    p[N - 2 - i] = tmp;
  }
}

void PrintArray(uint8_t* p, uint8_t N) {
  for (uint8_t i = 0; i < 2 * N; i++) {
    Serial.println(p[i]);
  }
  Serial.println("--------");
}
void Flow(CRGB* p, uint8_t* F, uint8_t N, uint32_t j, uint8_t RedorBlue, uint8_t Direc) {
  if (RedorBlue == 1) {
    if (Direc == 1) {
      for (uint32_t i = 0; i < N; i++) {
        p[i] = CRGB(F[(i + j) % (2 * N)], 0, 0);
      }
    } else {
      for (uint32_t i = 0; i < N; i++) {
        p[N - 1 - i] = CRGB(F[(i + j) % (2 * N)], 0, 0);
      }
    }
  } else {
    if (Direc == 1) {
      for (uint32_t i = 0; i < N; i++) {
        p[i] = CRGB(0, 0, F[(i + j) % (2 * N)]);
      }
    } else {
      for (uint32_t i = 0; i < N; i++) {
        p[N - 1 - i] = CRGB(0, 0, F[(i + j) % (2 * N)]);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.print("total:");
  Serial.println(N1 + N2 + N20 + N3 + N30 + N4 + N40 + N4_ + N5 + N50 + N6 + N60);

  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin20, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin30, OUTPUT);
  pinMode(Pin4, OUTPUT);
  pinMode(Pin40, OUTPUT);
  pinMode(Pin4_, OUTPUT);
  pinMode(Pin5, OUTPUT);
  pinMode(Pin50, OUTPUT);
  pinMode(Pin6, OUTPUT);
  pinMode(Pin60, OUTPUT);

  LEDS.addLeds<WS2812, Pin1, GRB>(L1, N1);
  LEDS.addLeds<WS2812, Pin2, GRB>(L2, N2);
  LEDS.addLeds<WS2812, Pin20, GRB>(L20, N20);
  LEDS.addLeds<WS2812, Pin3, GRB>(L3, N3);
  LEDS.addLeds<WS2812, Pin30, GRB>(L30, N30);
  LEDS.addLeds<WS2812, Pin4, GRB>(L4, N4);
  LEDS.addLeds<WS2812, Pin40, GRB>(L40, N40);
  LEDS.addLeds<WS2812, Pin4_, GRB>(L4_, N4_);
  LEDS.addLeds<WS2812, Pin5, GRB>(L5, N5);
  LEDS.addLeds<WS2812, Pin50, GRB>(L50, N50);
  LEDS.addLeds<WS2812, Pin6, GRB>(L6, N6);
  LEDS.addLeds<WS2812, Pin60, GRB>(L60, N60);

  //assign(F1, N1);  //F1赋值比较特殊
  double tmp;
  for (uint8_t i = 0; i < N1 / 4; i++) {
    tmp = 255.0 / ((N1 / 4) * (N1 / 4)) * i * i;
    F1[i] = tmp;
    F1[N1 / 2 - 2 - i] = tmp;
  }

  assign(F2, N2);
  assign(F20, N20);
  assign(F3, N3);
  assign(F30, N30);
  assign(F4, N4);
  assign(F40, N40);
  assign(F4_, N4_);
  assign(F5, N5);
  assign(F50, N50);
  assign(F6, N6);
  assign(F60, N60);

  // PrintArray(F1, N1);
  // PrintArray(F2, N2);
  // PrintArray(F2, N20);
  // PrintArray(F3, N3);
  // PrintArray(F30, N30);
  // PrintArray(F4, N4);
  // PrintArray(F40, N40);
  // PrintArray(F4_, N4_);
  // PrintArray(F5, N5);
  // PrintArray(F50, N50);
  // PrintArray(F6, N6);
  // PrintArray(F60, N60);
}

uint32_t j = 0;

void loop() {
  // Serial.println();

  // Flow(L1,F1,N1,j,1);
  Flow(L2, F2, N2, j, 1, 1);  // Red
  Flow(L3, F3, N3, j, 1, 1);
  Flow(L4, F4, N4, j, 1, 0);
  Flow(L5, F5, N5, j, 1, 0);
  Flow(L6, F6, N6, j, 1, 1);

  Flow(L20, F20, N20, j, 0, 0);  //Blue
  Flow(L30, F30, N30, j, 0, 0);
  Flow(L40, F40, N40, j, 0, 1);
  Flow(L4_, F4_, N4_, j, 0, 1);
  Flow(L50, F50, N50, j, 0, 1);
  Flow(L60, F60, N60, j, 0, 0);

  for (uint8_t i = 0; i < N1; i++) {
    if (i < N1 / 2 - 1) {
      L1[i] = CRGB(0, 0, F1[(i + j) % (N1)]);
    } else {
      L1[i] = CRGB(F1[(i - N1 / 2 + 1 + j) % (N1)], 0, 0);
    }
  }

  FastLED.show();
  delay(5);
  j++;

  // if (0 == j) {
  //   Serial.println("overflow");
  // }
}
