//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "HWR/picoX7/Config.h"

#if defined(HW_LCD_NONE)

enum Symbol : char
{
   SYMBOL_SINE      = '~',
   SYMBOL_TRIANGLE  = '^',
   SYMBOL_RAMP_UP   = '/',
   SYMBOL_RAMP_DN   = '\\',
   SYMBOL_PULSE     = 'n',
   SYMBOL_SQUARE    = 'S',
   SYMBOL_NOISE     = '@',
   SYMBOL_INFINITY  = '8'
};

#else

enum Symbol : unsigned char
{
   SYMBOL_SINE = 1,
   SYMBOL_TRIANGLE,
   SYMBOL_RAMP_UP,
   SYMBOL_RAMP_DN,
   SYMBOL_PULSE,
   SYMBOL_SQUARE,
   SYMBOL_NOISE,
   SYMBOL_INFINITY
};

extern void symbolDefine(HWR::Lcd& lcd);

#endif
