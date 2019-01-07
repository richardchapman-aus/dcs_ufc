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

#include "HidUfcMulti.h"


static const uint8_t _hidMultiReportDescriptorUfc[] PROGMEM = {
  /* HidUfc with 64 buttons*/
  0x05, 0x01,              /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x04,              /* USAGE (Joystick) */
  0xa1, 0x01,              /* COLLECTION (Application) */
  0x85, HID_REPORTID_UFC,      /*   REPORT_ID */
  /* 64 Buttons */
  0x05, 0x09,              /*   USAGE_PAGE (Button) */
  0x19, 0x01,              /*   USAGE_MINIMUM (Button 1) */
  0x29, 0x40,              /*   USAGE_MAXIMUM (Button 64) */
  0x15, 0x00,              /*   LOGICAL_MINIMUM (0) */
  0x25, 0x01,              /*   LOGICAL_MAXIMUM (1) */
  0x75, 0x01,              /*   REPORT_SIZE (1) */
  0x95, 0x40,              /*   REPORT_COUNT (64) */
  0x81, 0x02,              /*   INPUT (Data,Var,Abs) */
  0xc0                /* END_COLLECTION */
};

HidUfc_::HidUfc_(void)
{
  static HIDSubDescriptor node(_hidMultiReportDescriptorUfc, sizeof(_hidMultiReportDescriptorUfc));
  HID().AppendDescriptor(&node);
}


void HidUfc_::SendReport(void* data, int length)
{
  HID().SendReport(HID_REPORTID_UFC, data, length);
}

HidUfc_ HidUfc;
