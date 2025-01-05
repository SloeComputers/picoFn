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
#include "Table_triangle.h"
#include "Table_ramp_up.h"
#include "Table_ramp_dn.h"

#include "Table_delta.h"
#include "Table_freq.h"

#include "Symbol.h"

class Oscillator
{
public:
   enum Wave
   {
      SINE, TRIANGLE, PULSE, RAMP_UP, RAMP_DN, NOISE, NUM_WAVE
   };

   Oscillator()
   {
      setWave(SINE);
      setNote(69 - 12);
      setDetune(0);
      setPhase(0);
      setAmpl(128);
   }

   void getInfo(char* buffer16_) const
   {
      // Waveform
      Symbol wave_symbol;
      switch(wave)
      {
      case Oscillator::SINE:     wave_symbol = SYMBOL_SINE;     break;
      case Oscillator::TRIANGLE: wave_symbol = SYMBOL_TRIANGLE; break;
      case Oscillator::PULSE:    wave_symbol = SYMBOL_PULSE;    break;
      case Oscillator::RAMP_UP:  wave_symbol = SYMBOL_RAMP_UP;  break;
      case Oscillator::RAMP_DN:  wave_symbol = SYMBOL_RAMP_DN;  break;
      case Oscillator::NOISE:    wave_symbol = SYMBOL_NOISE;    break;
      default:                   wave_symbol = Symbol(' ');     break;
      }

      // Frequency
      char     freq_text[16];
      unsigned units = freqx1000 / 1000;
      if (units < 100)
      {
         unsigned frac = (freqx1000 % 1000) / 10;
         snprintf(freq_text, 16, "%2u.%02u ", units, frac);
      }
      else if (units < 1000)
      {
         unsigned frac = (freqx1000 % 1000) / 100;
         snprintf(freq_text, 16, "%3u.%u ", units, frac);
      }
      else if (units < 10000)
      {
         units = units / 1000;
         unsigned frac = (freqx1000 % 1000000) / 1000;
         snprintf(freq_text, 16, "%u.%03uk", units, frac);
      }
      else
      {
         units = units / 1000;
         unsigned frac = (freqx1000 % 1000000) / 10000;
         snprintf(freq_text, 16, "%2u.%02uk", units, frac);
      }

      // Musical note
      char     note_text[16];
      unsigned note   = exp_freq_hi7;
      unsigned detune = exp_freq_lo7;
      unsigned octave = note / 12;
      if ((detune == 0) && (octave < 10))
      {
         unsigned semitone = note % 12;
         const char* NOTE_LETTER = "CCDDEFFGGAAB";
         const char* NOTE_SHARP  = " # #  # # # ";
         snprintf(note_text, 16, "%c%u%c", NOTE_LETTER[semitone], octave, NOTE_SHARP[semitone]);
      }
      else
      {
         snprintf(note_text, 16, "-- ");
      }

      // Amplitude
      char     atten_text[16];
      unsigned atten = amp * 99 / 127;
      snprintf(atten_text, 16, "%2u", atten);

      snprintf(buffer16_, 16, "%c %6s %3s %2s%c", wave_symbol, freq_text, note_text, atten_text,
               SYMBOL_DB);
   }

   void sync()
   {
      phase_0_32 = 0;
   }

   void setWave(unsigned wave_)
   {
      if (wave_ < NUM_WAVE)
      {
         wave = Wave(wave_);
         setPhase(0);
         sync();
      }
   }

   void changeWave(signed delta)
   {
      if ((delta < 0) && (wave > SINE))
         setWave(wave - 1);
      else if ((delta > 0) && (wave < NOISE))
         setWave(wave + 1);
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

   void setAmpl(unsigned amp_)
   {
      amp = amp_;
   }

   void setPhase(unsigned phase_)
   {
      offset_0_32 = phase_ << 25;
   }

   //! Return next sample for this oscillator
   int32_t operator()()
   {
      phase_0_32 += delta_0_32;

      int32_t sample;

      switch(wave)
      {
      case SINE:     sample = sine();     break;
      case TRIANGLE: sample = triangle(); break;
      case PULSE:    sample = pulse();    break;
      case RAMP_UP:  sample = ramp_up();  break;
      case RAMP_DN:  sample = ramp_dn();  break;
      case NOISE:    sample = noise();    break;
      default:       sample = 0;          break;
      }

      return sample * amp / 0x80;
   }

private:
   int16_t sine() const
   {
      return table_sine[(phase_0_32 + offset_0_32) >> 16];
   }

   int16_t triangle() const
   {
      return table_triangle[phase_0_32 >> 16];
   }

   int16_t pulse () const
   {
      return phase_0_32 >= ((offset_0_32/2) + 0x80000000) ? 0x7FFF : -0x7FFF;
   }

   int16_t ramp_up() const
   {
      return table_ramp_up[phase_0_32 >> 16];
   }

   int16_t ramp_dn() const
   {
      return table_ramp_dn[phase_0_32 >> 16];
   }

   int16_t noise()
   {
      noise_state ^= noise_state << 13;
      noise_state ^= noise_state >> 17;
      noise_state ^= noise_state << 5;

      return noise_state;
   }

   void updateExpFreq()
   {
      unsigned index = (exp_freq_hi7 << 7) | exp_freq_lo7;
      delta_0_32 = table_delta[index];
      freqx1000  = table_freq[index];
   }

   Wave     wave{SINE};
   uint8_t  exp_freq_hi7{0};
   uint8_t  exp_freq_lo7{0};
   unsigned amp{};
   unsigned freqx1000{};

   uint32_t offset_0_32{0}; //!< Oscillator phase offset (x2pi) Q0.32
   uint32_t delta_0_32{0};  //!< Oscillator phase inc    (x2pi) Q0.32
   uint32_t phase_0_32{0};  //!< Oscillator phase        (x2pi) Q0.32

   uint32_t noise_state{1};
};
