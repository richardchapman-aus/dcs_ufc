#include <Arduino.h>
#include <SPI.h>
#include <Encoder.h>
#include "HidUfc.h"
//-- conf
#define DEBOUNCE_MS 10
#define SPI_HZ_74HC165 16000000
#define SPI_HZ_74HC595 16000000
//--pins
#define LED_G LED_BUILTIN
#define LED_Y 13
#define SPI_SS_74HC165 0
#define LATCH_74HC165 1
#define SPI_SS_74HC595 2
#define ENC_1_L 3
#define ENC_1_R 4
#define ENC_2_L 5
#define ENC_2_R 6

long posEnc1;
long posEnc2;
Encoder enc1(ENC_1_L,ENC_1_R);
Encoder enc2(ENC_2_L,ENC_2_R);

SPISettings SPI_74HC165(SPI_HZ_74HC165,MSBFIRST,SPI_MODE0);
SPISettings SPI_74HC595(SPI_HZ_74HC595,MSBFIRST,SPI_MODE0);

struct LoopState {
  byte btnReg1;
  byte btnReg2;
  byte btnReg3;
  byte keyPadM1;
  byte keyPadM2;
  byte keyPadM3;
  byte keyPadM4;
  bool enc1Left;
  bool enc1Right;
  bool enc2Left;
  bool enc2Right;
} LoopState;

void setup () {
  pinMode(SPI_SS_74HC165,OUTPUT);
  pinMode(LATCH_74HC165,OUTPUT);
  pinMode(SPI_SS_74HC595,OUTPUT);
  pinMode(ENC_1_L,INPUT_PULLUP);
  pinMode(ENC_1_R,INPUT_PULLUP);
  pinMode(ENC_2_L,INPUT_PULLUP);
  pinMode(ENC_2_R,INPUT_PULLUP);

  digitalWrite(SPI_SS_74HC165,HIGH);
  digitalWrite(LATCH_74HC165,HIGH);
  digitalWrite(SPI_SS_74HC595,HIGH);

  SPI.begin();
  HidUfc.begin();
}

void getButtonState () {
  digitalWrite(LATCH_74HC165,LOW);
  digitalWrite(LATCH_74HC165,HIGH);
  digitalWrite(SPI_SS_74HC165,LOW);
  SPI.beginTransaction(SPI_74HC165);
  LoopState.btnReg1 = SPI.transfer(0);
  LoopState.btnReg2 = SPI.transfer(0);
  LoopState.btnReg3 = SPI.transfer(0);
  SPI.transfer(0);//-- Clear keypad shift
  SPI.endTransaction();
  digitalWrite(SPI_SS_74HC165,HIGH);
}

void getKeypadState () {
  byte m,n;
  byte mask=0b00001111;

  for (m=0b00000001;m<=0b00001000;m<<=1) {
    //-- m
    digitalWrite(SPI_SS_74HC595,LOW);
    SPI.beginTransaction(SPI_74HC595);
    SPI.transfer(m);
    SPI.endTransaction();
    digitalWrite(SPI_SS_74HC595,HIGH);
    //-- n
    digitalWrite(LATCH_74HC165,LOW);
    digitalWrite(LATCH_74HC165,HIGH);
    digitalWrite(SPI_SS_74HC165,LOW);
    SPI.beginTransaction(SPI_74HC165);
    SPI.transfer(0);//-- Clear btn shift
    SPI.transfer(0);//-- Clear btn shift
    SPI.transfer(0);//-- Clear btn shift
    n = SPI.transfer(0);
    SPI.endTransaction();
    n>>=2;
    digitalWrite(SPI_SS_74HC165,HIGH);
    if (m==0b00001000) LoopState.keyPadM1 = ((m&mask)<<4|(n&mask));
    else if (m==0b00000100) LoopState.keyPadM2 = ((m&mask)<<4|(n&mask));
    else if (m==0b00000010) LoopState.keyPadM3 = ((m&mask)<<4|(n&mask));
    else if (m==0b00000001) LoopState.keyPadM4 = ((m&mask)<<4|(n&mask));
  }
}

void getEncoderState () {
  long curEnc1;
  long curEnc2;
  LoopState.enc1Left = false;
  LoopState.enc1Right = false;
  LoopState.enc2Left = false;
  LoopState.enc2Right = false;
  curEnc1 = enc1.read()/4;
  curEnc2 = enc2.read()/4;
  if (curEnc1 < posEnc1) LoopState.enc1Left = true;
  else if (curEnc1 > posEnc1) LoopState.enc1Right = true;
  if (curEnc2 < posEnc2) LoopState.enc2Left = true;
  else if (curEnc2 > posEnc2) LoopState.enc2Right = true;
  posEnc1 = curEnc1;
  posEnc2 = curEnc2;
}

void setHidUfcButton () {
  uint8_t m,i;
  //TODO: Best if distinct constant assignement
  for (m=0b10000000,i=1;m>0b00000000;m>>=1,i++) {
    (LoopState.btnReg1&m) ? HidUfc.press(i) : HidUfc.release(i);
    (LoopState.btnReg2&m) ? HidUfc.press(i+8) : HidUfc.release(i+8);
    (LoopState.btnReg3&m) ? HidUfc.press(i+16) : HidUfc.release(i+16);
  }
}

void setHidUfcEncoder () {
  (LoopState.enc1Left) ? HidUfc.press(UFC_ENC1_LEFT) : HidUfc.release(UFC_ENC1_LEFT);
  (LoopState.enc1Right) ? HidUfc.press(UFC_ENC1_RIGHT) : HidUfc.release(UFC_ENC1_RIGHT);
  (LoopState.enc2Left) ? HidUfc.press(UFC_ENC2_LEFT) : HidUfc.release(UFC_ENC2_LEFT);
  (LoopState.enc2Right) ? HidUfc.press(UFC_ENC2_RIGHT) : HidUfc.release(UFC_ENC2_RIGHT);
}

void setHidUfcKeypad () {
  uint8_t m;
  byte mskA1=0b00000001,mskA2=0b00000010,mskA3=0b00000100,mskA4=0b00001000;
  byte mskB1=0b00001000,mskB2=0b00000100,mskB3=0b00000010,mskB4=0b00000001;
  (LoopState.keyPadM1&mskA1) ? HidUfc.press(UFC_KEYPAD_1) : HidUfc.release(UFC_KEYPAD_1);
  (LoopState.keyPadM1&mskA2) ? HidUfc.press(UFC_KEYPAD_2) : HidUfc.release(UFC_KEYPAD_2);
  (LoopState.keyPadM1&mskA3) ? HidUfc.press(UFC_KEYPAD_3) : HidUfc.release(UFC_KEYPAD_3);
  (LoopState.keyPadM1&mskA4) ? HidUfc.press(UFC_KEYPAD_A) : HidUfc.release(UFC_KEYPAD_A);

  (LoopState.keyPadM1&mskA1) ? HidUfc.press(UFC_KEYPAD_4) : HidUfc.release(UFC_KEYPAD_4);
  (LoopState.keyPadM1&mskA2) ? HidUfc.press(UFC_KEYPAD_5) : HidUfc.release(UFC_KEYPAD_5);
  (LoopState.keyPadM1&mskA3) ? HidUfc.press(UFC_KEYPAD_6) : HidUfc.release(UFC_KEYPAD_6);
  (LoopState.keyPadM1&mskA4) ? HidUfc.press(UFC_KEYPAD_B) : HidUfc.release(UFC_KEYPAD_B);

  (LoopState.keyPadM1&mskB1) ? HidUfc.press(UFC_KEYPAD_7) : HidUfc.release(UFC_KEYPAD_7);
  (LoopState.keyPadM1&mskB2) ? HidUfc.press(UFC_KEYPAD_8) : HidUfc.release(UFC_KEYPAD_8);
  (LoopState.keyPadM1&mskB3) ? HidUfc.press(UFC_KEYPAD_9) : HidUfc.release(UFC_KEYPAD_9);
  (LoopState.keyPadM1&mskB4) ? HidUfc.press(UFC_KEYPAD_C) : HidUfc.release(UFC_KEYPAD_C);

  (LoopState.keyPadM1&mskB1) ? HidUfc.press(UFC_KEYPAD_STAR) : HidUfc.release(UFC_KEYPAD_STAR);
  (LoopState.keyPadM1&mskB2) ? HidUfc.press(UFC_KEYPAD_0) : HidUfc.release(UFC_KEYPAD_0);
  (LoopState.keyPadM1&mskB3) ? HidUfc.press(UFC_KEYPAD_HASH) : HidUfc.release(UFC_KEYPAD_HASH);
  (LoopState.keyPadM1&mskB4) ? HidUfc.press(UFC_KEYPAD_D) : HidUfc.release(UFC_KEYPAD_D);
}

void loop () {
  getButtonState();
  getEncoderState();
  getKeypadState();

  setHidUfcButton();
  setHidUfcEncoder();
  setHidUfcKeypad();

  HidUfc.write();
  delay(DEBOUNCE_MS);
}
