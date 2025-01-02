//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

#pragma once

#include <cstdint>

#include "Table_sine.h"
#include "Table_delta.h"
#include "Table_freq.h"

class Oscillator
{
public:
   Oscillator()
   {
      setNote(69 - 12);
      setDetune(0);
      setPhase(0);
      setAmpl(128);
   }

   uint8_t  getNote() const { return exp_freq_hi7; }

   uint8_t  getDetune() const { return exp_freq_lo7; }

   uint8_t  getAmpl() const { return ampl; }

   uint32_t getFreqx1000() const { return freqx1000; }

   void sync()
   {
      phase_0_32 = 0;
   }

   void setNote(uint8_t value_)
   {
      exp_freq_hi7 = value_;
      updateExpFreq();
   }

   void setDetune(uint8_t value_)
   {
      exp_freq_lo7 = value_;
      updateExpFreq();
   }

   void setAmpl(unsigned ampl_)
   {
      ampl = ampl_;
   }

   void setPhase(unsigned phase_)
   {
      offset_0_32 = phase_ << 25;
   }

   //! Return next sample for this oscillator
   int32_t operator()()
   {
      phase_0_32 += delta_0_32;

      return table_sine[(phase_0_32 + offset_0_32) >> 16] * ampl / 0x80;
   }

private:
   void updateExpFreq()
   {
      unsigned index = (exp_freq_hi7 << 7) | exp_freq_lo7;
      delta_0_32 = table_delta[index];
      freqx1000  = table_freq[index];
   }

   uint8_t  exp_freq_hi7{0};
   uint8_t  exp_freq_lo7{0};
   unsigned ampl{};
   unsigned freqx1000{};

   uint32_t offset_0_32{0}; //!< Oscillator phase offset (x2pi) Q0.32
   uint32_t delta_0_32{0};  //!< Oscillator phase inc    (x2pi) Q0.32
   uint32_t phase_0_32{0};  //!< Oscillator phase        (x2pi) Q0.32
};
