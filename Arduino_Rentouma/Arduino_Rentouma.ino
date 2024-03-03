#include <FastLED.h>

#define M1 9
#define M2 10
#define M3 11
#define M4 12
#define M5 13

#define L1 3
#define L2 4
#define L3 5
#define L4 6
#define L5 7

const uint8_t LED_NUM = 80;
const uint8_t LED_NUM_2 = LED_NUM >> 1;

CRGB Led1[LED_NUM], Led2[LED_NUM], Led3[LED_NUM], Led4[LED_NUM], Led5[LED_NUM];

CRGB Color1 = CRGB(255, 0, 0);
CRGB Color2 = CRGB(100, 0, 15);
CRGB Color3 = CRGB(190, 0, 115);
CRGB Color4 = CRGB(100, 0, 125);
CRGB Color5 = CRGB(100, 0, 190);

void setup() {
  Serial.begin(9600);
  pinMode(M1, INPUT_PULLUP);
  pinMode(M2, INPUT_PULLUP);
  pinMode(M3, INPUT_PULLUP);
  pinMode(M4, INPUT_PULLUP);
  pinMode(M5, INPUT_PULLUP);

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);

  LEDS.addLeds<WS2812, L1, GRB>(Led1, LED_NUM);
  LEDS.addLeds<WS2812, L2, GRB>(Led2, LED_NUM);
  LEDS.addLeds<WS2812, L3, GRB>(Led3, LED_NUM);
  LEDS.addLeds<WS2812, L4, GRB>(Led4, LED_NUM);
  LEDS.addLeds<WS2812, L5, GRB>(Led5, LED_NUM);
}

void Model3Func(CRGB LEDstripe, uint8_t num, CRGB color) {
}

void loop() {
  Serial.println(analogRead(A0));

  FastLED.clear();
  uint16_t Vol;
  while (1) {
    if (digitalRead(M1) == 0) {
      Vol = analogRead(A0);
      for (uint8_t i = 0; i < LED_NUM_2 && i < Vol / 6; i++) {
        Led1[LED_NUM_2 + i] = Color1;
        Led1[LED_NUM_2 - i] = Color1;

        Led2[LED_NUM_2 + i] = Color2;
        Led2[LED_NUM_2 - i] = Color2;

        Led3[LED_NUM_2 + i] = Color3;
        Led3[LED_NUM_2 - i] = Color3;

        Led4[LED_NUM_2 + i] = Color4;
        Led4[LED_NUM_2 - i] = Color4;

        Led5[LED_NUM_2 + i] = Color5;
        Led5[LED_NUM_2 - i] = Color5;
      }
      FastLED.show();
      break;
    }

    if (digitalRead(M2) == 0) {
      Vol = analogRead(A0);
      uint8_t n = Vol / 90;

      if (n > 0) {
        fill_solid(Led5, LED_NUM, Color5);
      }
      if (n > 1) {
        fill_solid(Led4, LED_NUM, Color4);
      }
      if (n > 2) {
        fill_solid(Led3, LED_NUM, Color3);
      }
      if (n > 3) {
        fill_solid(Led2, LED_NUM, Color2);
      }
      if (n > 4) {
        fill_solid(Led1, LED_NUM, Color1);
      }

      FastLED.show();
      break;
    }

    if (digitalRead(M3) == 0) {

      uint8_t delaytime = 200;


      // //-------------------
      // fill_solid(Led5, LED_NUM, CRGB(0, 0, 0));
      // FastLED.show();
      // if (digitalRead(M3) == 1) { break; }
      // delay(delaytime);

      // fill_solid(Led4, LED_NUM, CRGB(0, 0, 0));
      // FastLED.show();
      // if (digitalRead(M3) == 1) { break; }
      // delay(delaytime);

      // fill_solid(Led3, LED_NUM, CRGB(0, 0, 0));
      // FastLED.show();
      // if (digitalRead(M3) == 1) { break; }
      // delay(delaytime);

      // fill_solid(Led2, LED_NUM, CRGB(0, 0, 0));
      // FastLED.show();
      // if (digitalRead(M3) == 1) { break; }
      // delay(delaytime);

      // fill_solid(Led1, LED_NUM, CRGB(0, 0, 0));
      // FastLED.show();
      // if (digitalRead(M3) == 1) { break; }
      // delay(delaytime + 300);
      ///////////////////////---------
      fill_solid(Led5, LED_NUM, Color5);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led4, LED_NUM, Color4);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led3, LED_NUM, Color3);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led2, LED_NUM, Color2);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led1, LED_NUM, Color1);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime + 300);
      //-------------------
      fill_solid(Led5, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led4, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led3, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led2, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led1, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime + 300);
      ///---------------
      fill_solid(Led1, LED_NUM, Color1);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led2, LED_NUM, Color2);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led3, LED_NUM, Color3);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led4, LED_NUM, Color4);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led5, LED_NUM, Color5);
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime + 300);
      //---------------
      fill_solid(Led1, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led2, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led3, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led4, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime);

      fill_solid(Led5, LED_NUM, CRGB(0, 0, 0));
      FastLED.show();
      if (digitalRead(M3) == 1) { break; }
      delay(delaytime + 300);

      break;
    }

    if (digitalRead(M4) == 0) {

      fill_solid(Led1, LED_NUM, Color1);
      fill_solid(Led2, LED_NUM, Color2);
      fill_solid(Led3, LED_NUM, Color3);
      fill_solid(Led4, LED_NUM, Color4);
      fill_solid(Led5, LED_NUM, Color5);

      FastLED.show();
      break;
    }

    if (digitalRead(M5) == 0) {
      FastLED.clear();
      FastLED.show();
      break;
    }
  }
}
