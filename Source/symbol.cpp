//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include "Symbol.h"

#if not defined(HW_LCD_NONE)

static const uint8_t BITMAP_SINE[8] =
{
   0b00000,
   0b00000,
   0b01000,
   0b10101,
   0b00010,
   0b00000,
   0b00000,
   0b00000
};

static const uint8_t BITMAP_TRIANGLE[8] =
{
   0b00000,
   0b00000,
   0b00100,
   0b01010,
   0b10001,
   0b00000,
   0b00000,
   0b00000
};

static const uint8_t BITMAP_PULSE[8] =
{
   0b00000,
   0b00000,
   0b11100,
   0b10100,
   0b10100,
   0b10111,
   0b00000,
   0b00000
};

static const uint8_t BITMAP_RAMP_UP[8] =
{
   0b00000,
   0b00001,
   0b00011,
   0b00101,
   0b01001,
   0b10001,
   0b00000,
   0b00000
};

static const uint8_t BITMAP_RAMP_DN[8] =
{
   0b00000,
   0b10000,
   0b11000,
   0b10100,
   0b10010,
   0b10001,
   0b00000,
   0b00000
};

static const uint8_t BITMAP_NOISE[8] =
{
   0b00010,
   0b01000,
   0b01001,
   0b01100,
   0b10101,
   0b10010,
   0b01011,
   0b00000
};

static const uint8_t BITMAP_DB[8] =
{
   0b00110,
   0b00101,
   0b00101,
   0b01110,
   0b10101,
   0b10101,
   0b01110,
   0b00000
};

static const uint8_t BITMAP_INFINITY[8] =
{
   0b00000,
   0b00000,
   0b01010,
   0b10101,
   0b01010,
   0b00000,
   0b00000,
   0b00000
};

void symbolDefine(hw::Lcd& lcd)
{
   lcd.progChar(SYMBOL_SINE,     BITMAP_SINE);
   lcd.progChar(SYMBOL_TRIANGLE, BITMAP_TRIANGLE);
   lcd.progChar(SYMBOL_PULSE,    BITMAP_PULSE);
   lcd.progChar(SYMBOL_RAMP_UP,  BITMAP_RAMP_UP);
   lcd.progChar(SYMBOL_RAMP_DN,  BITMAP_RAMP_DN);
   lcd.progChar(SYMBOL_NOISE,    BITMAP_NOISE);
   lcd.progChar(SYMBOL_DB,       BITMAP_DB);
   lcd.progChar(SYMBOL_INFINITY, BITMAP_INFINITY);
}

#endif
