//*//
#include <Arduino.h>
#include <SPI.h>
#include <Encoder.h>
#include "HID-Project.h"
//-- conf
#define DEBOUNCE_MS 100//10
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

long posEnc1;
long posEnc2;
Encoder enc1(ENC_1_L,ENC_1_R);
Encoder enc2(ENC_2_L,ENC_2_R);

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
  Gamepad.begin();
}

void getBtnState () {
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

void getRotaryState () {
  LoopState.enc1Left = false;
  LoopState.enc1Right = false;
  LoopState.enc2Left = false;
  LoopState.enc2Right = false;
  long curEnc1 = enc1.read()/4;
  long curEnc2 = enc2.read()/4;
  if (curEnc1 < posEnc1) LoopState.enc1Left = true;
  else if (curEnc1 > posEnc1) LoopState.enc1Right = true;
  if (curEnc2 < posEnc2) LoopState.enc2Left = true;
  else if (curEnc2 > posEnc2) LoopState.enc2Right = true;
  posEnc1 = curEnc1;
  posEnc2 = curEnc2;
}

void setGamepadBtn () {
  uint8_t m,i;
  for (m=0b10000000,i=1;m>0b00000000;m>>=1,i++) {
    (LoopState.btnReg1&m) ? Gamepad.press(i) : Gamepad.release(i);
    (LoopState.btnReg2&m) ? Gamepad.press(i+8) : Gamepad.release(i+8);
    (LoopState.btnReg3&m) ? Gamepad.press(i+16) : Gamepad.release(i+16);
  }
}

void setGamepadKeypad () {

}

void setGamepadEnc () {
  (LoopState.enc1Left) ? Gamepad.press(15) : Gamepad.release(15);
  (LoopState.enc1Right) ? Gamepad.press(16) : Gamepad.release(16);
  (LoopState.enc2Left) ? Gamepad.press(9) : Gamepad.release(9);
  (LoopState.enc2Right) ? Gamepad.press(25) : Gamepad.release(25);
}

void loop () {
  //digitalWrite(LED_G,HIGH);
  //digitalWrite(LED_Y,HIGH);

  getBtnState();
  getKeypadState();
  getRotaryState();

  setGamepadBtn();
  setGamepadKeypad();
  setGamepadEnc();

  // Move x/y Axis to a new position (16bit)
  Gamepad.xAxis(random(0xFFFF));
  Gamepad.yAxis(random(0xFFFF));

  // Go through all dPad positions
  // values: 0-8 (0==centered)
  static uint8_t dpad1 = GAMEPAD_DPAD_CENTERED;
  Gamepad.dPad1(dpad1++);
  if (dpad1 > GAMEPAD_DPAD_UP_LEFT)
    dpad1 = GAMEPAD_DPAD_CENTERED;

  static int8_t dpad2 = GAMEPAD_DPAD_CENTERED;
  Gamepad.dPad2(dpad2--);
  if (dpad2 < GAMEPAD_DPAD_CENTERED)
    dpad2 = GAMEPAD_DPAD_UP_LEFT;

  // Functions above only set the values.
  // This writes the report to the host.
  Gamepad.write();

  delay(DEBOUNCE_MS);
  digitalWrite(LED_Y,LOW);
}
//*/
