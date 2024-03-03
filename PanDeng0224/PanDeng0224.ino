#include "CRC16.h"
CRC16 crc;

#define ENPIN 4
#define DATAPIN 3
#define CLKPIN 2

#define ANT_Q1 22
#define ANT_Q2 24
#define ANT_Q3 26
#define ANT_Q4 28

#define BUTTON_PIN1 30
#define BUTTON_PIN2 32
#define BUTTON_PIN3 34
#define BUTTON_PIN4 36
#define BUTTON_PIN5 38
#define BUTTON_PIN6 40


// #define Enable() \
//   { \
//     digitalWrite(ENPIN, 1); \
//     delayMicroseconds(5); \
//     digitalWrite(ENPIN, 0); \
//     delayMicroseconds(5); \
//   }
#define Enable() \
  { \
    digitalWrite(ENPIN, 1); \
    delayMicroseconds(3); \
    digitalWrite(ENPIN, 0); \
  }
// Enable()中的delayMicroseconds似乎不能太长（或许不能超过10）

bool GoOutD = false;
char LastCommand, NewCommand = 'D';

#define ReceiveCOM() \
  { \
    if ((1 == digitalRead(ANT_Q1)) || (1 == digitalRead(BUTTON_PIN1))) { \
      NewCommand = 'A'; \
    } \
    if ((1 == digitalRead(ANT_Q2)) || (1 == digitalRead(BUTTON_PIN2))) { \
      NewCommand = 'B'; \
    } \
    if ((1 == digitalRead(ANT_Q3)) || (1 == digitalRead(BUTTON_PIN3))) { \
      NewCommand = 'C'; \
    } \
    if ((1 == digitalRead(ANT_Q4)) || (1 == digitalRead(BUTTON_PIN4))) { \
      NewCommand = 'D'; \
      GoOutD = false; \
    } \
    if (1 == digitalRead(BUTTON_PIN5)) { \
      NewCommand = 'E'; \
    } \
    if (1 == digitalRead(BUTTON_PIN6)) { \
      NewCommand = 'F'; \
    } \
    if (Serial.available()) { \
      NewCommand = (char)Serial.read(); \
    } \
    if (NewCommand != LastCommand) { \
      LastCommand = NewCommand; \
      Serial.println(String("Command:") + String(NewCommand)); \
      if (NewCommand != 'D') { \
        GoOutD = true; \
      } \
      return; \
    } \
  }


inline void setVolt(uint16_t V) {  //1600
  // Serial1.write(0x01);             //address
  // Serial1.write(0x06);             //03, 06
  // Serial1.write(0x00);
  // Serial1.write(0x30);  //start address
  crc.restart();
  crc.add(0x01);
  crc.add(0x06);
  crc.add(0x00);
  crc.add(0x30);
  //////////////
  uint8_t VH = V >> 8;
  uint8_t VL = V & 0xff;
  Serial1.write(VH);
  Serial1.write(VL);  //data
  crc.add(VH);
  crc.add(VL);

  uint16_t c = crc.getCRC();
  Serial1.write(c & 0xff);
  Serial1.write(c >> 8);
}


#define DW(x) \
  { \
    digitalWrite(CLKPIN, 0); \
    digitalWrite(DATAPIN, (x)); \
    delayMicroseconds(3); \
    digitalWrite(CLKPIN, 1); \
    delayMicroseconds(3); \
  }

// #define DW(x) \
//   { \
//     digitalWrite(CLKPIN, 0); \
//     digitalWrite(DATAPIN, (x)); \
//     digitalWrite(CLKPIN, 1); \
//   }

inline void Send_Byte(byte txd) {
  DW(txd & 0x80);
  DW(txd & 0x40);
  DW(txd & 0x20);
  DW(txd & 0x10);
  DW(txd & 0x8);
  DW(txd & 0x4);
  DW(txd & 0x2);
  DW(txd & 0x1);
}

inline void Send_4Byte(uint32_t txd) {
  Send_Byte((byte)(txd >> 24));
  Send_Byte((byte)(txd >> 16));
  Send_Byte((byte)(txd >> 8));
  Send_Byte((byte)(txd));
}

inline void Send_3Byte(uint32_t txd) {
  Send_Byte((byte)(txd >> 16));
  Send_Byte((byte)(txd >> 8));
  Send_Byte((byte)(txd));
}

inline void Send_SameByte(uint8_t a, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    Send_Byte(a);
  }
}
#define Breath() \
  { \
    if (j % dj == 0) { \
      if (j <= (n_2)) { \
        setVolt((uint16_t)(((float)j) / n * 2 * (L2 - L1) + L1)); \
      } else { \
        setVolt((uint16_t)(-((float)j) / n * 2 * (L2 - L1) + 2 * L2 - L1)); \
      } \
    } \
  }


uint16_t L1GlobalBreath = 1400, L2GlobalBreath = 1800;
#define LightnessIncrease(deltaLight, deltaTime) \
  { \
    for (uint16_t i = L1GlobalBreath; i <= L2GlobalBreath; i += (deltaLight)) { \
      ReceiveCOM(); \
      setVolt(i); \
      delay((deltaTime)); \
    } \
  }
#define LightnessDecrease(deltaLight, deltaTime) \
  { \
    for (uint16_t i = L2GlobalBreath; i >= L1GlobalBreath; i -= (deltaLight)) { \
      ReceiveCOM(); \
      setVolt(i); \
      delay((deltaTime)); \
    } \
  }

void setup() {
  pinMode(ENPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  pinMode(CLKPIN, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);

  crc.setPolynome(0x8005);
  crc.setStartXOR(0xffff);
  crc.setEndXOR(0x0000);
  crc.setReverseIn(true);
  crc.setReverseOut(true);
}


void ExtinguishAll() {  //熄灭全部
  Send_SameByte(0x00, 10);
  Enable();
}

void LightUpAll() {  //全亮
  Send_SameByte(0xff, 10);
  Enable();
}



long time;


float L1 = 1400, L2 = 1800;
void loop() {

  if (NewCommand == 'A') {  //逐列扫描,呼吸灭，中间向两边扩散
    // Serial.println("in ifA");
    time = millis();
    Model_A();
    Serial.print("A:");
    Serial.println(millis() - time);
    return;
  }

  if (NewCommand == 'B') {  //7大域
    // Serial.println("in ifB");
    time = millis();
    Model_B();
    Serial.print("B:");
    Serial.println(millis() - time);
    return;
  }

  if (NewCommand == 'C') {  // 汇聚到中央
    // Serial.println("in ifC");
    time = millis();
    Model_C();
    Serial.print("C:");
    Serial.println(millis() - time);
    return;
  }

  if (NewCommand == 'D') {  //A,B,C 3种模式循环
    // ReceiveCOM();
    // LightUpAll();
    setVolt(1800);

    GoOutD = false;
    Model_A();
    if (GoOutD) {
      return;
    }
    delay(100);

    Model_B();
    if (GoOutD) {
      return;
    }
    delay(100);

    Model_C();
    if (GoOutD) {
      return;
    }
    delay(100);
  }

  if (NewCommand == 'E') {
    ReceiveCOM();
    LightUpAll();
    setVolt(1800);
  }

  if (NewCommand == 'F') {
    ReceiveCOM();
    LightUpAll();
    L1 = 1200;
    L2 = 1900;
    LightnessIncrease(8, 30);
    LightnessDecrease(8, 30);
  }

  if (NewCommand == 'G') {  //检测“行”的MOS管是否损坏（变成始终接通的状态）
    ReceiveCOM();
    Send_SameByte(0x00, 3);
    Send_SameByte(0xff, 7);
    Enable();
    return;
  }
  if (NewCommand == 'H') {  //检测“列”的MOS管是否损坏（变成始终接通的状态）
    ReceiveCOM();
    Send_SameByte(0xff, 3);
    Send_SameByte(0x00, 7);
    Enable();
    return;
  }
  // delay(300);
}

void Model_A() {  //逐列扫描,呼吸灭，中间向两边扩散
  uint16_t dt = 100;
  ExtinguishAll();
  delay(300);

  uint32_t t1 = 1;
  for (byte i = 0; i < 32; ++i) {
    ReceiveCOM();
    setVolt(1800);

    Send_SameByte(0xff, 3);

    Send_SameByte(0x00, 3);
    Send_4Byte(t1);

    Enable();
    t1 = (t1 << 1) | 1;
    delay(dt);
  }
  t1 = 1;
  for (byte i = 0; i < 18; ++i) {
    ReceiveCOM();
    Send_SameByte(0xff, 3);

    Send_3Byte(t1);
    Send_SameByte(0xff, 4);

    Enable();
    t1 = (t1 << 1) | 1;
    delay(dt);
  }

  LightnessDecrease(8, 30);

  // //中间向两边扩散
  dt = 120;
  ExtinguishAll();
  // delay(300);
  uint32_t tt1 = 0x00000003, tt2 = 0x00000000;
  for (byte i = 0; i < 23; ++i) {
    ReceiveCOM();
    setVolt(1800);
    Send_SameByte(0xff, 3);

    Send_Byte(0x00);
    Send_3Byte(tt1);
    Send_3Byte(tt2);

    Enable();
    tt1 = (tt1 << 1) | 0x00000007;
    tt2 = (tt2 >> 1) | 0x00800000;
    delay(dt);
  }
  {
    ReceiveCOM();
    Send_SameByte(0xff, 3);
    Send_Byte(0x01);
    Send_3Byte(0x00ffffff);
    Send_3Byte(0x00fffffe);
    Enable();
    delay(dt);
  }
  {
    ReceiveCOM();
    Send_SameByte(0xff, 3);
    Send_Byte(0x03);
    Send_3Byte(0x00ffffff);
    Send_3Byte(0x00ffffff);
    Enable();
    delay(dt);
  }
  LightnessDecrease(8, 30);
  LightnessIncrease(8, 30);
  LightnessDecrease(8, 30);
}

void Model_B() {  // 7大域
  setVolt(1000);
  delay(200);
  ExtinguishAll();
  setVolt(1200);
  delay(100);  // 防止正式亮之前亮一下

  uint16_t n, n_2;
  uint16_t dt = 10;
  L1 = 1200;
  L2 = 1900;
  byte dj = 30;  // if (j % 10 == 0)  dj=10

  n = 1000;
  n_2 = n >> 1;                       // n_2=n/2
  for (uint16_t j = 0; j < n; ++j) {  //1,回形，数字座舱
    ReceiveCOM();

    Send_3Byte(0x00038070);  // send_byte(0x03),0x80,0x70
    Send_Byte(0x00);
    Send_Byte(0x01);
    Send_SameByte(0xff, 3);
    Send_Byte(0x80);
    Send_Byte(0x00);
    Enable();

    Send_3Byte(0x0003fff0);
    Send_Byte(0x00);
    Send_Byte(0x01);
    Send_Byte(0xfc);
    Send_Byte(0x00);
    Send_Byte(0x3f);
    Send_Byte(0x80);
    Send_Byte(0x00);
    Enable();

    Breath();
  }
  delay(dt);

  n = 1400;
  n_2 = n >> 1;
  for (uint16_t j = 0; j < n; ++j) {  //2,矩形，中央域
    ReceiveCOM();

    Send_3Byte(0x00007f80);

    Send_SameByte(0x00, 2);
    Send_Byte(0x03);
    Send_Byte(0xff);
    Send_Byte(0xc0);
    Send_SameByte(0x00, 2);
    Enable();

    Breath();
  }
  delay(dt);

  n = 1200;
  n_2 = n >> 1;
  for (uint16_t j = 0; j < n; ++j) {  //3,左右两竖条，集成安全域
    ReceiveCOM();

    Send_3Byte(0x00ffffff);
    Send_Byte(0x03);
    Send_SameByte(0x00, 5);
    Send_Byte(0x03);
    Enable();

    Breath();
  }
  delay(dt);

  n = 1200;
  n_2 = n >> 1;
  for (uint16_t j = 0; j < n; ++j) {  //4,矩形，智能驾驶域
    ReceiveCOM();

    Send_3Byte(0x0003fff0);

    Send_SameByte(0x00, 5);
    Send_Byte(0x7f);
    Send_Byte(0xfc);
    Enable();

    Breath();
  }
  delay(dt);

  n = 1200;
  n_2 = n >> 1;
  for (uint16_t j = 0; j < n; ++j) {  //5,矩形，车身域
    ReceiveCOM();

    Send_3Byte(0x000c000c);

    Send_Byte(0x00);
    Send_SameByte(0xff, 5);
    Send_Byte(0xfc);
    Enable();

    Breath();
  }
  delay(dt);

  n = 1200;
  n_2 = n >> 1;
  for (uint16_t j = 0; j < n; ++j) {  //矩形，新能源域
    ReceiveCOM();

    Send_3Byte(0x0003fff0);
    Send_Byte(0x00);
    Send_Byte(0xfe);
    Send_SameByte(0x00, 5);
    Enable();

    Breath();
  }
  delay(dt);

  n = 1200;
  n_2 = n >> 1;
  for (uint16_t j = 0; j < n; ++j) {  //7,矩形，底盘域
    ReceiveCOM();

    Send_3Byte(0x00300003);
    Send_SameByte(0xff, 7);
    Enable();

    Breath();
  }
  delay(dt);

  LightUpAll();
  LightUpAll();  //防止全亮不成功
  LightnessIncrease(8, 30);
  LightnessDecrease(8, 30);
  LightnessIncrease(8, 30);
  LightnessDecrease(8, 30);
}

void Model_C() {
  // uint16_t n = 60;
  uint16_t dt = 90;

  uint32_t t1 = 0x00200001, t2 = 0x03000000, t3 = 0x03;
  for (uint8_t i = 0; i < 11; ++i) {
    setVolt(1800);
    for (uint8_t j = 0; j < 30; ++j) {  //因为要快速交替，此for循环不可删除，不可换成delay
      ReceiveCOM();

      Send_3Byte(t1);
      Send_SameByte(0xff, 7);
      Enable();
      // delay(2);
      Send_SameByte(0xff, 3);
      Send_Byte(0x03);
      Send_3Byte(t2);
      Send_3Byte(t3);
      Enable();
    }
    t1 = ((t1 >> 1) | t1) | ((t1 << 1) | t1);
    t2 = (t2 >> 2) | t2;
    t3 = (t3 << 2) | t3;
    // delay(dt);
  }
  LightUpAll();
  delay(10);
  LightUpAll();

  for (uint16_t i = 1800; i > 1400; i -= 10) {
    ReceiveCOM();
    setVolt(i);
    delay(20);
  }
  // 然后从中间向四周扩散
  t1 = 0x00000C00, t2 = 0x00000007, t3 = 0x00800000;
  for (uint8_t i = 0; i < 11; ++i) {
    setVolt(1800);

    ReceiveCOM();

    Send_3Byte(t1);  // 行

    Send_4Byte(t2);
    Send_3Byte(t3);
    Enable();
    delay(120);

    t1 = ((t1 >> 1) | t1) | ((t1 << 1) | t1);
    t2 = (t2 << 2) | t2;
    t3 = (t3 >> 2) | 0x00C00000;
    // delay(dt);
  }
  LightUpAll();
  delay(10);
  LightUpAll();

  LightnessDecrease(8, 30);
  LightnessIncrease(8, 30);
  LightnessDecrease(8, 30);
}
