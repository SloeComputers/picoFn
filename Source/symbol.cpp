//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
   0b01100,
   0b01100,
   0b01100,
   0b11111,
   0b00000,
   0b00000
};

static const uint8_t BITMAP_SQUARE[8] =
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

static const uint8_t BITMAP_INFINITY[8] =
{
   0b00000,
   0b00000,
   0b01010,
   0b10101,
   0b10101,
   0b01010,
   0b00000,
   0b00000
};

void symbolDefine(HWR::Lcd& lcd)
{
   lcd.progChar(SYMBOL_SINE,      BITMAP_SINE);
   lcd.progChar(SYMBOL_TRIANGLE,  BITMAP_TRIANGLE);
   lcd.progChar(SYMBOL_RAMP_UP,   BITMAP_RAMP_UP);
   lcd.progChar(SYMBOL_RAMP_DN,   BITMAP_RAMP_DN);
   lcd.progChar(SYMBOL_PULSE,     BITMAP_PULSE);
   lcd.progChar(SYMBOL_SQUARE,    BITMAP_SQUARE);
   lcd.progChar(SYMBOL_NOISE,     BITMAP_NOISE);
   lcd.progChar(SYMBOL_INFINITY,  BITMAP_INFINITY);
}

#endif
