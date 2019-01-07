/*
Copyright (c) 2014-2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include guard
#pragma once

#include <Arduino.h>
#include "HidUfcSettings.h"

//-- Button numbers
#define UFC_MASTER_WARNING 10
#define UFC_MASTER_CAUTION 11
#define UFC_FIVEOPTS_1 4
#define UFC_FIVEOPTS_2 3
#define UFC_FIVEOPTS_3 2
#define UFC_FIVEOPTS_4 1
#define UFC_FIVEOPTS_5 12
#define UFC_NAVCOM_1 17
#define UFC_NAVCOM_2 18
#define UFC_NAVCOM_3 19
#define UFC_NAVCOM_4 20
#define UFC_NAVCOM_5 24
#define UFC_NAVCOM_6 23
#define UFC_NAVCOM_7 22
#define UFC_NAVCOM_8 21
#define UFC_ENC1_PRESS 14
#define UFC_ENC1_LEFT 15
#define UFC_ENC1_RIGHT 16
#define UFC_ENC2_PRESS 13
#define UFC_ENC2_LEFT 9
#define UFC_ENC2_RIGHT 25
#define UFC_CRS_LEFT 8
#define UFC_CRS_RIGHT 7
#define UFC_HDG_LEFT 6
#define UFC_HDG_RIGHT 5
#define UFC_KEYPAD_1 26
#define UFC_KEYPAD_2 27
#define UFC_KEYPAD_3 28
#define UFC_KEYPAD_4 29
#define UFC_KEYPAD_5 30
#define UFC_KEYPAD_6 31
#define UFC_KEYPAD_7 32
#define UFC_KEYPAD_8 33
#define UFC_KEYPAD_9 34
#define UFC_KEYPAD_0 35
#define UFC_KEYPAD_STAR 36
#define UFC_KEYPAD_HASH 37
#define UFC_KEYPAD_A 38
#define UFC_KEYPAD_B 39
#define UFC_KEYPAD_C 40
#define UFC_KEYPAD_D 41

typedef union {
  uint64_t whole64[0];
  uint64_t buttons;


	struct{
		uint8_t button1 : 1;
		uint8_t button2 : 1;
		uint8_t button3 : 1;
		uint8_t button4 : 1;
		uint8_t button5 : 1;
		uint8_t button6 : 1;
		uint8_t button7 : 1;
		uint8_t button8 : 1;

		uint8_t button9 : 1;
		uint8_t button10 : 1;
		uint8_t button11 : 1;
		uint8_t button12 : 1;
		uint8_t button13 : 1;
		uint8_t button14 : 1;
		uint8_t button15 : 1;
		uint8_t button16 : 1;

		uint8_t button17 : 1;
		uint8_t button18 : 1;
		uint8_t button19 : 1;
		uint8_t button20 : 1;
		uint8_t button21 : 1;
		uint8_t button22 : 1;
		uint8_t button23 : 1;
		uint8_t button24 : 1;

		uint8_t button25 : 1;
		uint8_t button26 : 1;
		uint8_t button27 : 1;
		uint8_t button28 : 1;
		uint8_t button29 : 1;
		uint8_t button30 : 1;
		uint8_t button31 : 1;
		uint8_t button32 : 1;

		uint8_t button33 : 1;
		uint8_t button34 : 1;
		uint8_t button35 : 1;
		uint8_t button36 : 1;
		uint8_t button37 : 1;
		uint8_t button38 : 1;
		uint8_t button39 : 1;
		uint8_t button40 : 1;

		uint8_t button41 : 1;
		uint8_t button42 : 1;
		uint8_t button43 : 1;
		uint8_t button44 : 1;
		uint8_t button45 : 1;
		uint8_t button46 : 1;
		uint8_t button47 : 1;
		uint8_t button48 : 1;

		uint8_t button49 : 1;
		uint8_t button50 : 1;
		uint8_t button51 : 1;
		uint8_t button52 : 1;
		uint8_t button53 : 1;
		uint8_t button54 : 1;
		uint8_t button55 : 1;
		uint8_t button56 : 1;

		uint8_t button57 : 1;
		uint8_t button58 : 1;
		uint8_t button59 : 1;
		uint8_t button60 : 1;
		uint8_t button61 : 1;
		uint8_t button62 : 1;
		uint8_t button63 : 1;
		uint8_t button64 : 1;
	};
} HID_UfcReport_Data_t;

class HidUfcApi{
public:
	inline HidUfcApi(void);

	inline void begin(void);
	inline void end(void);
	inline void write(void);
	inline void press(uint8_t b);
	inline void release(uint8_t b);
	inline void releaseAll(void);
	inline void buttons(uint64_t b);

	// Sending is public in the base class for advanced users.
	virtual void SendReport(void* data, int length) = 0;

protected:
	HID_UfcReport_Data_t _report;
};

// Implementation is inline
#include "HidUfcApi.hpp"
