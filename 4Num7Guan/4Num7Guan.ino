#include <DS3231.h>
#include <FastLED.h>
#include <avr/wdt.h>
//#include <SoftwareSerial.h>

// The analog input pins can be used as digital pins, referred to as A0, A1, etc.
// The exception is the Arduino Nano, Pro Mini, and Mini’s A6 and A7 pins,
// which can only be used as analog inputs.

#define Q1 2
#define Q2 3
#define Q3 4
#define Q4 5
#define Q5 6
#define Q6 7
#define Q7 8
#define Q8 9

#define L1 12
#define L2 13
// L1:转圈   L2：数字
#define N1 72
#define N2 58

#define ReceiveCOM() \
  { \
    if (1 == digitalRead(Q1)) { \
      NewCommand = 1; \
    } \
    if (1 == digitalRead(Q2)) { \
      NewCommand = 2; \
      FirstIn = 1; \
      CommandTime = millis(); \
    } \
    if (1 == digitalRead(Q3)) { \
      NewCommand = 3; \
      FirstIn = 1; \
      CommandTime = millis(); \
    } \
    if (1 == digitalRead(Q4)) { \
      NewCommand = 4; \
      FirstIn = 1; \
      CommandTime = millis(); \
    } \
    if (1 == digitalRead(Q5)) { \
      NewCommand = 5; \
      FirstIn = 1; \
      CommandTime = millis(); \
    } \
    if (1 == digitalRead(Q6)) { \
      NewCommand = 6; \
      FirstIn = 1; \
      CommandTime = millis(); \
    } \
    if (NewCommand != LastCommand) { \
      LastCommand = NewCommand; \
      return; \
    } \
  }


//SoftwareSerial BT(10, 11);
//char val;

CRGB Led1[N1], Led2[N2];

uint8_t flow[N1] = { 0 };

uint8_t a3[11][7] = { 1, 1, 1, 0, 1, 1, 1,
                      1, 0, 0, 0, 1, 0, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      1, 1, 0, 1, 1, 1, 0,
                      1, 0, 0, 1, 1, 0, 1,
                      1, 1, 0, 1, 0, 1, 1,
                      1, 1, 1, 1, 0, 1, 1,
                      1, 0, 0, 0, 1, 1, 0,
                      1, 1, 1, 1, 1, 1, 1,
                      1, 1, 0, 1, 1, 1, 1,
                      0, 0, 0, 0, 0, 0, 0 };

uint8_t a4[11][7] = { 0, 1, 1, 1, 1, 1, 1,
                      0, 0, 0, 1, 1, 0, 0,
                      1, 1, 1, 0, 1, 1, 0,
                      1, 0, 1, 1, 1, 1, 0,
                      1, 0, 0, 1, 1, 0, 1,
                      1, 0, 1, 1, 0, 1, 1,
                      1, 1, 1, 1, 0, 1, 1,
                      0, 0, 0, 1, 1, 1, 0,
                      1, 1, 1, 1, 1, 1, 1,
                      1, 0, 1, 1, 1, 1, 1,
                      0, 0, 0, 0, 0, 0, 0 };

uint8_t Led2_14 = 0;

// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

long millisecond, CommandTime;
uint8_t LastCommand, NewCommand = 0;

void setup() {
  LEDS.addLeds<WS2812, L1, RGB>(Led1, N1);
  LEDS.addLeds<WS2812, L2, RGB>(Led2, N2);

  // Setup Serial connection
  Serial.begin(9600);
  //BT.begin(9600);
  //Serial.println("Bluetooth ready");
  // Uncomment the next line if you are using an Arduino Leonardo
  //while (!Serial) {}

  rtc.begin();
  //rtc.setTime(17,25,10);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);

  for (uint8_t j = 0; j < 24; j++) {
    flow[j] = 255 - j * 7;
  }

  millisecond = millis();
  NewCommand = 0;
  wdt_enable(WDTO_8S);
}
// L1:转圈   L2：数字

uint16_t i = 0;
long waitTime = 25000;
uint8_t FirstIn = 1;
Time t;
uint8_t H[7] = { 0, 0, 1, 2, 4, 5, 7 };  //需要显示的小时的数组，前2个元素不用。

void loop() {
  wdt_reset();

  ReceiveCOM();
  //Serial.println(NewCommand);

  if (NewCommand == 0) {  //显示北京时间
    t = rtc.getTime();
    DispNum((t.hour < 10 ? 10 : t.hour / 10), (t.hour % 10), (t.min < 10 ? 0 : t.min / 10), (t.min % 10));
    FastLED.show();
    return;
  }

  if (NewCommand == 1) {  // 1,中间0~24h快速增加，外圈旋转
    DispNum(i / 100, (i / 10) % 10, i % 10, 0);

    if (i % 6 == 0) {
      for (uint8_t j = 0; j < 72; j += 2) {
        Led1[j] = CRGB(flow[(j + i / 2) % N1], 0, 0);
        Led1[j + 1] = Led1[j];
      }
    }
    i = (i + 1) % 241;
    FastLED.show();
    delay(10);
    return;
  }

  if (NewCommand == 2 || NewCommand == 3 || NewCommand == 4 || NewCommand == 5 || NewCommand == 6) {  //显示1、2、4、5、7点
    DispNum(10, H[NewCommand], 0, 0);
    FastLED.show();
    if (1 == FirstIn) {  //闪3次
      //CommandTime=millis();
      // Serial.print("FirstInTime:");
      // Serial.println(CommandTime);

      uint16_t dt = 600;
      for (uint8_t k = 0; k < 3; k++) {
        ReceiveCOM();
        fill_solid(Led1, N1, CRGB(255, 0, 0));
        DispNum(10, H[NewCommand], 0, 0);
        FastLED.show();
        delay(dt);

        ReceiveCOM();
        fill_solid(Led1, N1, CRGB(0, 0, 0));
        DispNum(10, H[NewCommand], 0, 0);
        FastLED.show();
        delay(dt);
      }
      FirstIn = 0;
    }
    if (millis() > CommandTime + waitTime) {
      NewCommand = 0;
      Serial.print("20s passed:");
      Serial.println(millis());
    }
    return;
  }
  
}

void DispNum(uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4) {
  for (uint8_t i = 0; i < 13; i += 2) {
    Led2[i] = CRGB(255 * a4[n4][i >> 1], 0, 0);
    Led2[i + 1] = Led2[i];
    /////
    Led2[i + 14] = CRGB(255 * a3[n3][i >> 1], 0, 0);
    Led2[i + 15] = Led2[i + 14];
    /////
    Led2[i + 30] = CRGB(255 * a4[n2][i >> 1], 0, 0);
    Led2[i + 31] = Led2[i + 30];
    /////
    Led2[i + 44] = CRGB(255 * a3[n1][i >> 1], 0, 0);
    Led2[i + 45] = Led2[i + 44];
  }
  if (millis() > millisecond + 500) {
    Led2_14 = 1 - Led2_14;
    millisecond = millis();
  }
  Led2[28] = CRGB(255 * Led2_14, 0, 0);
  Led2[29] = Led2[28];
}
