//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "Table_sine.h"
#include "Table_triangle.h"
#include "Table_ramp_up.h"
#include "Table_ramp_dn.h"
#include "Table_delta.h"
#include "Table_freq_mHz.h"
#include "Table_amp.h"
#include "Table_atten_cB.h"
#include "Table_const.h"

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
      setNote(69 - 8);
      setDetune(0);
      setPhase(0);
      setAmpl(127);
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
      char     freq_text[7];
      unsigned units = freq_mHz / 1000;
      if (units < 100)
      {
         unsigned frac = (freq_mHz % 1000) / 10;
         snprintf(freq_text, sizeof(freq_text), "%2u.%02u ", units, frac);
      }
      else if (units < 1000)
      {
         unsigned frac = (freq_mHz % 1000) / 100;
         snprintf(freq_text, sizeof(freq_text), "%3u.%u ", units, frac);
      }
      else if (units < 10000)
      {
         units = units / 1000;
         unsigned frac = (freq_mHz % 1000000) / 1000;
         snprintf(freq_text, sizeof(freq_text), "%u.%03uk", units, frac);
      }
      else
      {
         units = units / 1000;
         unsigned frac = (freq_mHz % 1000000) / 10000;
         snprintf(freq_text, sizeof(freq_text), "%2u.%02uk", units, frac);
      }

      // Musical note
      char     note_text[4];
      signed   note   = exp_freq_hi7 - 4;
      unsigned detune = exp_freq_lo7;
      signed   octave = note / 12;
      if ((detune == 0) && (octave < 10))
      {
         unsigned semitone = (note + 12) % 12;
         const char* NOTE_LETTER = "CCDDEFFGGAAB";
         const char* NOTE_SHARP  = " # #  # # # ";

         if (octave == -1)
            snprintf(note_text, sizeof(note_text), "%c%c%c",
                     NOTE_LETTER[semitone], SYMBOL_MINUS_ONE, NOTE_SHARP[semitone]);
         else
            snprintf(note_text, sizeof(note_text), "%c%u%c",
                     NOTE_LETTER[semitone], octave, NOTE_SHARP[semitone]);
      }
      else
      {
         snprintf(note_text, sizeof(note_text), "-- ");
      }

      // Amplitude
      char atten_text[5];
      unsigned atten_cb  = table_atten_cB[amp_index];
      unsigned db        = atten_cb / 10;
      unsigned db_tenths = atten_cb % 10;
      if (atten_cb == ATTEN_INFINITY)
         snprintf(atten_text, sizeof(atten_text), "   %c", SYMBOL_INFINITY);
      else if (atten_cb == 0)
         snprintf(atten_text, sizeof(atten_text), "   0");
      else
         snprintf(atten_text, sizeof(atten_text), "%2u.%u", db, db_tenths);


      snprintf(buffer16_, 17, "%c %6s %3s%4s",
               wave_symbol, freq_text, note_text, atten_text);
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

   void changeWave(signed delta_)
   {
      if ((delta_ < 0) && (wave > SINE))
         setWave(wave - 1);
      else if ((delta_ > 0) && (wave < NOISE))
         setWave(wave + 1);
   }

   void setNote(uint8_t value7_)
   {
      exp_freq_hi7 = value7_;
      updateExpFreq();
   }

   void setDetune(uint8_t value7_)
   {
      exp_freq_lo7 = value7_;
      updateExpFreq();
   }

   void setAmpl(unsigned amp7_)
   {
      amp_index = amp7_;
      amp       = table_amp[amp7_];
   }

   void setPhase(unsigned phase7_)
   {
      offset_0_32 = phase7_ << (PHASE_BITS - 7);
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

      return sample * amp / 0x10000;
   }

private:
   int16_t sine() const
   {
      return table_sine[(phase_0_32 + offset_0_32) >> PHASE_SHIFT];
   }

   int16_t triangle() const
   {
      return table_triangle[phase_0_32 >> PHASE_SHIFT];
   }

   int16_t pulse () const
   {
      const uint32_t PHASE_PI = 1 << (PHASE_BITS - 1);
      return phase_0_32 >= (PHASE_PI + (offset_0_32 / 2)) ? +SAMPLE_MAX
                                                          : -SAMPLE_MAX;
   }

   int16_t ramp_up() const
   {
      return table_ramp_up[phase_0_32 >> PHASE_SHIFT];
   }

   int16_t ramp_dn() const
   {
      return table_ramp_dn[phase_0_32 >> PHASE_SHIFT];
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
      unsigned index = (exp_freq_hi7 << FREQ_FRAC_BITS) | exp_freq_lo7;
      delta_0_32 = table_delta[index];
      freq_mHz   = table_freq_mHz[index];
   }

   static const unsigned PHASE_SHIFT = PHASE_BITS - LOG2_WAVE_SIZE;

   Wave     wave{SINE};
   uint8_t  exp_freq_hi7{0};
   uint8_t  exp_freq_lo7{0};
   uint8_t  amp_index{};
   unsigned amp{};
   unsigned freq_mHz{};

   uint32_t offset_0_32{0}; //!< Oscillator phase offset (x2pi) Q0.32
   uint32_t delta_0_32{0};  //!< Oscillator phase inc    (x2pi) Q0.32
   uint32_t phase_0_32{0};  //!< Oscillator phase        (x2pi) Q0.32

   uint32_t noise_state{1};
};
