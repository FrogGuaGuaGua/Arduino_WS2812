#include "FastLED.h"
#include "Chrono.h"

#define ReceiveCOM() \
  { \
    if (Serial.available()) { \
      NewCommand = (char)Serial.read(); \
      if (NewCommand != LastCommand) { \
        LastCommand = NewCommand; \
        Serial.println(String("A") + String(NewCommand)); \
        return; \
      } \
    } \
  }
Chrono ChronoA, ChronoB;
const uint8_t N2 = 30, NMNO = 30, NB = 20, NC = 32, ND = 5, NE = 5, NF = 10;
const uint8_t NA = 10, NG = 6, NH = 6, NI1 = 5, NI2 = 5, NJ = 3, NK = 4;
//以上为不同灯带的灯珠数量，对于流动效果的灯带，需要精确的灯珠数量；
//对于纯色的灯带，程序中的灯珠数量大于等于真实灯珠数量即可，无需精确。

CRGB L2[N2], LMNO[NMNO], LB[NB], LC[NC], LD[ND], LE[NE], LF[NF], LA[NA], LG[NG];
CRGB LH[NH], LI1[NI1], LI2[NI2], LJ[NJ], LK[NK], LL1[1], LL2[1], LL3[1], LL4[1];

CRGB L1_4Color = CRGB(230, 255, 230);

struct {
  const uint8_t h = 7;
  const uint8_t s = 250;
} MyRed;
struct {
  const uint8_t h = 140;
  const uint8_t s = 250;
} MyBlue;
struct {
  const uint8_t h = 18;
  const uint8_t s = 240;
} MyYellow;
struct {
  const uint8_t h = 100;
  const uint8_t s = 240;
} MyGreen;
// 以上结构体中的h和s，都是用于HSV颜色中的H和S.

const uint8_t ArrayAxisLen = NK;  // NK = 4
uint8_t flowArrayAxis[ArrayAxisLen] = { 255, 255, 50, 0 };

const uint8_t ArrayHighWireLen = NC;  // NC = 32
uint8_t flowArrayHighWire[ArrayHighWireLen] = { 0 };
uint8_t flowArrayHighWireInv[ArrayHighWireLen] = { 255, 255, 0 };
// flowArrayHighWire数组，前面都是0，最后两个元素是255(在void setup()中进行赋值)；
// flowArrayHighWireInv数组，前两个元素255，后面都是0.

const uint8_t ArrayWheelLen = NA;  // NA = 10
uint8_t flowArrayWheel[ArrayWheelLen];

const uint8_t BriLen = 150;
uint8_t Bri[BriLen] = { 0, 40, 80, 120, 160, 200, 240, 255 };
uint8_t WheelBri[BriLen] = { 0, 30, 60, 90, 120, 150, 180, 180 };
// 存储渐亮亮度的数组，255之后的元素也全是255，在void setup()中进行赋值。

bool BreathPlusMinus;
uint8_t BreathBright = 0;

char LastCommand, NewCommand;

long tempMillis;
int j1, j2;
uint8_t WheelDelayTime;

void setup() {
  LEDS.addLeds<WS2812, 31, GRB>(L2, N2);  // 初始化光带
  LEDS.addLeds<WS2812, 3, GRB>(LMNO, NMNO);
  LEDS.addLeds<WS2812, 4, GRB>(LB, NB);
  LEDS.addLeds<WS2812, 5, GRB>(LC, NC);
  LEDS.addLeds<WS2812, 6, GRB>(LD, ND);
  LEDS.addLeds<WS2812, 7, GRB>(LE, NE);
  LEDS.addLeds<WS2812, 8, GRB>(LF, NF);

  LEDS.addLeds<WS2812, 34, GRB>(LA, NA);

  LEDS.addLeds<WS2812, 23, GRB>(LG, NG);
  LEDS.addLeds<WS2812, 25, GRB>(LH, NH);
  LEDS.addLeds<WS2812, 27, GRB>(LI1, NI1);
  LEDS.addLeds<WS2812, 29, GRB>(LI2, NI2);
  LEDS.addLeds<WS2812, 22, GRB>(LJ, NJ);
  LEDS.addLeds<WS2812, 24, GRB>(LK, NK);
  LEDS.addLeds<WS2812, 26, GRB>(LL1, 1);
  LEDS.addLeds<WS2812, 28, GRB>(LL2, 1);
  LEDS.addLeds<WS2812, 30, GRB>(LL3, 1);
  LEDS.addLeds<WS2812, 32, GRB>(LL4, 1);

  FastLED.setBrightness(255);  // 设置光带亮度，范围为0 – 255
  LastCommand = '0';
  NewCommand = '0';
  Serial.begin(19200);

  for (int i = 0; i < ArrayWheelLen; ++i) {
    flowArrayWheel[i] = 25 * i;
  }

  flowArrayHighWire[ArrayHighWireLen - 2] = 255;
  flowArrayHighWire[ArrayHighWireLen - 1] = 255;

  for (int i = 8; i < BriLen; ++i) {
    Bri[i] = 255;
    WheelBri[i] = 180;
  }
}

void loop() {
  FastLED.clear();
  FastLED.show();
  switch (NewCommand) {
    case '0':  // 全部熄灭
      ReceiveCOM();
      // delay(10);
      // Serial.println("0");
      break;
    case '1':  // 纯电模式
      tempMillis = millis();

      for (uint8_t j = 0; j < 52; ++j) {
        ReceiveCOM();
        ChunDian(j, true, 15);
        delay(50);
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '2':  // 增程模式
      tempMillis = millis();

      for (uint8_t j = 0; j < 52; ++j) {
        ReceiveCOM();
        ZengCheng(j, true, true);
        delay(50);
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '3':  //  混动模式
      tempMillis = millis();

      for (uint8_t j = 0; j < 53; ++j) {
        ReceiveCOM();
        HunDong(j, true);
        delay(50);
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '4':  //  能量回收模式
      tempMillis = millis();

      BreathBright = 50;
      for (uint8_t j = 0; j < 53; ++j) {
        ReceiveCOM();
        HuiShou(j, true);
        delay(50);
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '5':  //  能量回收模式，长时间7s
      tempMillis = millis();
      BreathBright = 50;
      for (uint8_t j = 0; j < 122; ++j) {
        ReceiveCOM();
        HuiShou(j, true);
        delay(50);
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '6':  //  快，纯电2s+增程5s
      tempMillis = millis();

      j1 = 0, j2 = 0;
      WheelDelayTime = 60;
      while (j1 < 116) {
        ReceiveCOM();

        if (ChronoA.hasPassed(60)) {
          ChronoA.restart();
          if (j1 < 33) {
            ChunDian(j1, false, 0);
          } else {
            fill_solid(LL1, 1, CRGB(0, 0, 0));  // 模式灯，四选一
            ZengCheng(j1 - 33, false, false);
          }
          ++j1;
        }

        if (ChronoB.hasPassed(WheelDelayTime)) {
          ChronoB.restart();
          if (j2 > 0) {
            if (WheelDelayTime > 20) {
              WheelDelayTime -= 1;
            }
            for (int i = 0; i < NA; ++i) {
              LA[i] = CHSV(MyBlue.h, flowArrayWheel[(i + j2) % ArrayWheelLen], WheelBri[(j2 < 8 ? j2 : 10)]);
            }  // 上一行改成j2-10之类的。
               //WheelRotate(j2);
          }
          ++j2;
        }
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;

    case '7':  //  静，纯电5s+增程7s
      tempMillis = millis();
      j1 = 0, j2 = 0;
      while (j1 < 101) {
        ReceiveCOM();

        if (ChronoA.hasPassed(60)) {
          ChronoA.restart();
          ChunDian(j1++, false, 0);
        }
        if (ChronoB.hasPassed(60)) {
          ChronoB.restart();
          ++j2;
          if (j2 > 19) {
            WheelRotate(j2 - 19);
          }
        }
      }
      Serial.println(millis() - tempMillis);
      ////////////
      FastLED.clear();
      j1 = 0, j2 = 0;
      while (j1 < 101) {
        ReceiveCOM();
        if (ChronoA.hasPassed(60)) {
          ChronoA.restart();
          ZengCheng(j1++, false, true);
        }
        if (ChronoB.hasPassed(60)) {
          ChronoB.restart();
          ++j2;
          if (j2 > 26) {
            WheelRotate(j2 - 26);
          }
        }
      }
      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '8':  //  3.5s+3s+3s+5s+2s+3.5s
      tempMillis = millis();
      j1 = 0, j2 = 0;
      WheelDelayTime = 60;
      BreathBright = 50;
      while (j1 < 241) {
        ReceiveCOM();

        if (ChronoA.hasPassed(60)) {
          ChronoA.restart();
          if (j1 < 58) {
            ChunDian(j1, false, 0);
          } else if (j1 == 58) {
            FastLED.clear();
          } else if (j1 < 108) {
            ZengCheng(j1 - 58, false, false);
          } else if (j1 == 108) {
            FastLED.clear();
          } else if (j1 < 158) {
            HunDong(j1 - 108, false);
          } else if (j1 == 158) {
            FastLED.clear();
          } else {
            HuiShou(j1 - 158, false);
          }
          ++j1;
        }

        if (ChronoB.hasPassed(60)) {
          ChronoB.restart();
          if (j2 > 0 && j2 < 158) {  // 把j2>0改成j2>5之类的，可改变点亮时刻，注意下面CHSV中也要改成j2-5
            for (int i = 0; i < NA; ++i) {
              LA[i] = CHSV(MyBlue.h, flowArrayWheel[(i + j2) % ArrayWheelLen], WheelBri[(j2 < 8 ? j2 : 10)]);
            }
          } else {
            for (int i = 0; i < NA; ++i) {
              LA[i] = CHSV(MyGreen.h, flowArrayWheel[(i + j2) % ArrayWheelLen], 200);
            }
          }
          ++j2;
        }
      }

      FastLED.clear();
      FastLED.show();
      delay(2000);

      for (uint8_t j = 0; j < 80; ++j) {
        ReceiveCOM();
        ChunDian(j, true, 0);
        delay(50);
      }

      Serial.println(millis() - tempMillis);
      Serial.println("e");
      break;
    case '9':  //  待机，车轮和两个大灯呼吸
      BreathBright = 50;
      while (1) {
        ReceiveCOM();

        if (BreathBright >= 250) {
          BreathPlusMinus = false;
        }
        if (BreathBright <= 70) {
          BreathPlusMinus = true;
        }
        fill_solid(LA, NA, CHSV(0, 0, BreathBright));                  // 车轮，呼吸
        fill_solid(L2, N2, CHSV(MyBlue.h, MyBlue.s, BreathBright));    // 纯电大灯，呼吸
        fill_solid(LMNO, NMNO, CHSV(MyRed.h, MyRed.s, BreathBright));  // 燃油大灯，呼吸
        if (BreathPlusMinus == true) { BreathBright += 4; }
        if (BreathPlusMinus == false) { BreathBright -= 4; }

        FastLED.show();
        delay(10);
      }
      break;
    case 'a':  // 轮播1,2,3,4四种模式
      while (1) {
        for (uint8_t j = 0; j < 52; ++j) {
          ReceiveCOM();
          ChunDian(j, true, 15);
          delay(50);
        }
        FastLED.clear();

        for (uint8_t j = 0; j < 52; ++j) {
          ReceiveCOM();
          ZengCheng(j, true, true);
          delay(50);
        }
        FastLED.clear();

        for (uint8_t j = 0; j < 53; ++j) {
          ReceiveCOM();
          HunDong(j, true);
          delay(50);
        }
        FastLED.clear();

        BreathBright = 50;
        for (uint8_t j = 0; j < 53; ++j) {
          ReceiveCOM();
          HuiShou(j, true);
          delay(50);
        }
        FastLED.clear();
      }
      break;
  }
}

void WheelRotate(uint8_t j) {
  for (int i = 0; i < NA; ++i) {
    LA[i] = CHSV(MyBlue.h, flowArrayWheel[(i + j) % ArrayWheelLen], WheelBri[j]);
  }  // LA 车轮蓝、白光旋转
}

void ChunDian(uint8_t j, bool boolWheel, uint8_t LeftMotorStart) {
  fill_solid(LL1, 1, L1_4Color);                         // 纯电模式灯，四选一
  fill_solid(L2, N2, CHSV(MyBlue.h, MyBlue.s, Bri[j]));  //L2,蓝色，音盆右上角大灯
  fill_solid(LB, NB, CHSV(MyBlue.h, MyBlue.s, Bri[j]));  //LB,蓝色，电池8个灯

  if (boolWheel && j > 19) {
    for (int i = 0; i < NA; ++i) {
      LA[i] = CHSV(MyBlue.h, flowArrayWheel[(i + j) % ArrayWheelLen], WheelBri[j - 19]);
    }  // LA 车轮蓝、白光旋转
  }

  for (uint8_t i = 0; i < NC; ++i) {
    LC[i] = CHSV(MyBlue.h, MyBlue.s, flowArrayHighWire[(i + j * 2) % ArrayHighWireLen]);
  }             //电池到集成电机控制器，蓝色，保持 流动
  if (j > 5) {  // 集成电机控制器，蓝色，渐亮，落后于前面的流动
    fill_solid(LD, ND, CHSV(MyBlue.h, MyBlue.s, Bri[j - 5]));
  }
  if (j > LeftMotorStart) {  //LH，黄色，左侧大电机灯   // LeftMotorStart一般取15
    fill_solid(LH, NH, CHSV(MyYellow.h, MyYellow.s, Bri[j - LeftMotorStart]));
  }
  if (j > (LeftMotorStart + 2)) {  //LI1，黄色，左侧大电机轴灯、齿轮灯
    fill_solid(LJ, NJ, CHSV(0, 0, Bri[j - 17]));
    fill_solid(LI1, NI1, CHSV(MyYellow.h, MyYellow.s, Bri[j - (LeftMotorStart + 2)]));
  }
  FastLED.show();
}

void ZengCheng(uint8_t j, bool boolWheel, bool boolHighWireInv) {
  fill_solid(LL2, 1, L1_4Color);  // 模式灯，四选一
  fill_solid(L2, N2, CHSV(MyBlue.h, MyBlue.s, Bri[j]));
  fill_solid(LMNO, NMNO, CHSV(MyRed.h, MyRed.s, Bri[j]));

  if (j > 4) {
    fill_solid(LB, NB, CHSV(MyBlue.h, MyBlue.s, Bri[j - 4]));
    fill_solid(LF, NF, CHSV(MyRed.h, MyRed.s, Bri[j - 4]));
    fill_solid(LE, NE, CHSV(80, 0, Bri[j - 4]));
  }

  if (j > 6) {
    for (uint8_t i = 0; i < NK; ++i) {  // LK, 小电机轴灯，流动
      LK[i] = CHSV(0, 0, flowArrayAxis[(i + (j - 6)) % ArrayAxisLen]);
    }
  }

  if (j > 7) {
    fill_solid(LG, NG, CHSV(MyYellow.h, MyYellow.s, Bri[j - 7]));  // LG, 右侧小电机灯
  }
  //fill_solid(LI2, NI2, CHSV(MyYellow.h, MyYellow.s, 250));   // 右侧小电机齿轮灯


  if ((j > 5 && j < 31) || (!boolHighWireInv && j > 30)) {
    for (uint8_t i = 0; i < NC; ++i) {
      LC[i] = CHSV(MyBlue.h, MyBlue.s, flowArrayHighWire[(i + (j - 5) * 3) % ArrayHighWireLen]);
    }  //电池到集成电机控制器，蓝色，保持 流动
  }

  if (j > 10) {  // 集成电机控制器，蓝色，渐亮，落后于前面的流动
    fill_solid(LD, ND, CHSV(MyBlue.h, MyBlue.s, Bri[j - 10]));
  }
  if (j > 23) {  //LH，黄色，左侧大电机灯
    fill_solid(LH, NH, CHSV(MyYellow.h, MyYellow.s, Bri[j - 23]));
  }
  if (j > 25) {  //LI1，黄色，左侧大电机轴灯、齿轮灯
    fill_solid(LJ, NJ, CHSV(0, 0, Bri[j - 17]));
    fill_solid(LI1, NI1, CHSV(MyYellow.h, MyYellow.s, Bri[j - 25]));
  }

  if (boolWheel && j > 26) {
    for (int i = 0; i < NA; ++i) {
      LA[i] = CHSV(MyBlue.h, flowArrayWheel[(i + j) % ArrayWheelLen], WheelBri[j - 26]);
    }  // LA 车轮蓝、白光旋转
  }

  if (boolHighWireInv && j > 30) {  // 高压线灯光反流
    fill_solid(LC, NC, CHSV(0, 0, 0));
    for (uint8_t i = 0; i < NC; ++i) {
      LC[NC - i] = CHSV(MyBlue.h, MyBlue.s, flowArrayHighWireInv[(i + (j - 30) * 3) % ArrayHighWireLen]);
    }  //电池到集成电机控制器，蓝色，保持 流动
  }

  //delay(delayTime);
  FastLED.show();
  //}
}

void HunDong(uint8_t j, bool boolWheel) {
  fill_solid(LL3, 1, L1_4Color);  // 混动模式灯，四选一
  fill_solid(L2, N2, CHSV(MyBlue.h, MyBlue.s, Bri[j]));
  fill_solid(LMNO, NMNO, CHSV(MyRed.h, MyRed.s, Bri[j]));
  fill_solid(LB, NB, CHSV(MyBlue.h, MyBlue.s, Bri[j]));

  for (uint8_t i = 0; i < NC; ++i) {
    LC[i] = CHSV(MyBlue.h, MyBlue.s, flowArrayHighWire[(i + j * 2) % ArrayHighWireLen]);
  }  //电池到集成电机控制器，蓝色，保持 流动

  if (j > 6) {  // 集成电机控制器，蓝色，渐亮，落后于前面的流动
    fill_solid(LD, ND, CHSV(MyBlue.h, MyBlue.s, Bri[j - 6]));
  }
  if (j > 10) {  //LH，黄色，左侧大电机灯
    fill_solid(LH, NH, CHSV(MyYellow.h, MyYellow.s, Bri[j - 10]));
  }
  if (j > 12) {                                   //LI1，黄色，左侧大电机轴灯、齿轮灯
    fill_solid(LJ, NJ, CHSV(0, 0, Bri[j - 12]));  //左电机轴
    fill_solid(LI1, NI1, CHSV(MyYellow.h, MyYellow.s, Bri[j - 12]));
  }
  if (j > 15) {  // LF，活塞4灯  LE,发动机管理系统，一排小灯
    fill_solid(LF, NF, CHSV(MyRed.h, MyRed.s, Bri[j - 15]));
    fill_solid(LE, NE, CHSV(0, 0, Bri[j - 15]));
  }
  if (j > 18) {  // 小电机齿轮灯
    fill_solid(LI2, NI2, CHSV(MyYellow.h, MyYellow.s, Bri[j - 18]));
  }
  if (j > 22) {  // 小电机灯
    // fill_solid(LG, NG, CHSV(MyYellow.h, MyYellow.s, Bri[j - 22]));
    // for (uint8_t i = 0; i < NK; ++i) {  // LK, 小电机轴灯，流动
    //   LK[i] = CHSV(0, 0, flowArrayAxis[(i + j) % ArrayAxisLen]);
    // }
    fill_solid(LK, NK, CHSV(0, 0, Bri[j - 22]));
  }
  if (boolWheel && j > 13) {  //车轮
    for (int i = 0; i < NA; ++i) {
      LA[i] = CHSV(MyBlue.h, flowArrayWheel[(i + j) % ArrayWheelLen], WheelBri[j - 13]);
    }
  }
  FastLED.show();
}

void HuiShou(uint8_t j, bool boolWheel) {
  fill_solid(LL4, 1, L1_4Color);  // 模式灯，四选一
  if (boolWheel && j > 3) {
    for (int i = 0; i < NA; ++i) {
      LA[i] = CHSV(MyGreen.h, flowArrayWheel[(i + j) % ArrayWheelLen], WheelBri[j - 3]);
    }
  }

  if (j > 3) {  // 左电机齿轮和轴，渐亮
    fill_solid(LJ, NJ, CHSV(0, 0, Bri[j - 3]));
    fill_solid(LI1, NI1, CHSV(MyYellow.h, MyYellow.s, Bri[j - 3]));
  }

  if (j > 6) {
    if (BreathBright >= 250) {
      BreathPlusMinus = false;
    }
    if (BreathBright <= 70) {
      BreathPlusMinus = true;
    }
    fill_solid(LH, NH, CHSV(MyYellow.h, MyYellow.s, BreathBright));  // 左侧大电机灯，呼吸
    if (BreathPlusMinus == true) { BreathBright += 20; }
    if (BreathPlusMinus == false) { BreathBright -= 20; }
  }
  if (j > 9) {
    for (uint8_t i = 0; i < NC; ++i) {
      LC[NC - i] = CHSV(MyBlue.h, MyBlue.s, flowArrayHighWireInv[(i + (j - 9) * 2) % ArrayHighWireLen]);
    }
  }
  if (j > 15) {  // LD,集成控制器
    fill_solid(LD, ND, CHSV(MyBlue.h, MyBlue.s, Bri[j - 15]));
  }
  if (j > 18) {  // LB,电池包
    fill_solid(LB, NB, CHSV(MyBlue.h, MyBlue.s, Bri[j - 15]));
  }
  if (j > 23) {  // L2, 右上角纯电大灯
    fill_solid(L2, N2, CHSV(MyBlue.h, MyBlue.s, Bri[j - 23]));
  }

  FastLED.show();
}
