//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "SIG/SIG.h"

#include "Table_freq_mHz.h"
#include "Table_atten_cB.h"

#include "Symbol.h"

class Oscillator
{
public:
   enum Wave
   {
      SINE, TRIANGLE, RAMP_UP, RAMP_DN, SQUARE, PULSE, NOISE, NUM_WAVE
   };

   Oscillator()
   {
      setWave(SINE);
      setDetune(0);
      setPhase(0);
      setAmpl(127);

      noteOn(69, 0);
   }

   void getInfo(char* buffer16_) const
   {
      // Waveform
      Symbol wave_symbol;
      switch(wave)
      {
      case Oscillator::SINE:     wave_symbol = SYMBOL_SINE;     break;
      case Oscillator::TRIANGLE: wave_symbol = SYMBOL_TRIANGLE; break;
      case Oscillator::RAMP_UP:  wave_symbol = SYMBOL_RAMP_UP;  break;
      case Oscillator::RAMP_DN:  wave_symbol = SYMBOL_RAMP_DN;  break;
      case Oscillator::SQUARE:   wave_symbol = SYMBOL_SQUARE;   break;
      case Oscillator::PULSE:    wave_symbol = SYMBOL_PULSE;    break;
      case Oscillator::NOISE:    wave_symbol = SYMBOL_NOISE;    break;
      default:                   wave_symbol = Symbol(' ');     break;
      }

      // Frequency
      char     freq_text[7];
      unsigned index    = ((note - 8 ) << 7) | detune7;
      unsigned freq_mHz = table_freq_mHz[index];
      unsigned units    = freq_mHz / 1000;
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
      char   note_text[4];
      signed octave = (note / 12) - 1;
      if (detune7 == 0)
      {
         unsigned semitone = (note + 12) % 12;
         const char* NOTE_LETTER = "CCDDEFFGGAAB";
         const char* NOTE_SHARP  = " # #  # # # ";

         if ((octave < 0) || (octave > 9))
            snprintf(note_text, sizeof(note_text), "%c %c",
                     NOTE_LETTER[semitone], NOTE_SHARP[semitone]);
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
      if (atten_cb == 0xFFFF)
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
      for(unsigned i = 0; i < 6; ++i)
         osc[i]->sync();
   }

   void setWave(unsigned wave_)
   {
      if (wave_ < NUM_WAVE)
      {
         wave = Wave(wave_);
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

   void setDetune(uint8_t detune7_)
   {
      detune7 = detune7_;

      updateFreq();
   }

   void setAmpl(uint8_t amp7_)
   {
      amp_index = amp7_;

      gain = SIG::dBAttenLookup_7(127 - amp7_);
   }

   void setPhase(uint8_t phase7_)
   {
      SIG::UPhase phase = phase7_ << (sizeof(SIG::UPhase) * 8 - 7);

      for(unsigned i = 0; i < (6 - 2); ++i)
         osc[i]->setPhase(phase + phase_ref.getPhase());

      pulse.setWidth(-float(phase7_ - 64) / 65.0f);
   }

   void noteOn(uint8_t note_, uint8_t velocity_ = 0)
   {
      note = note_;
      updateFreq();
   }

   //! Return next sample for this oscillator
   SIG::Signal operator()()
   {
      SIG::Signal sample;

      SIG::UPhase delta = noteLookup(unsigned(note_slew()));
      for(unsigned i = 0; i < 6; ++i)
         osc[i]->setDelta(delta);

      (void) phase_ref();

      switch(wave)
      {
      case SINE:     sample = sine();     break;
      case TRIANGLE: sample = triangle(); break;
      case RAMP_UP:  sample = ramp();     break;
      case RAMP_DN:  sample = -ramp();    break;
      case SQUARE:   sample = square();   break;
      case PULSE:    sample = pulse();    break;
      case NOISE:    sample = noise();    break;
      default:       sample = 0.0f;       break;
      }

      return gain(sample);
   }

private:
   static SIG::UPhase noteLookup(unsigned note_7_)
   {
      if (note_7_ < 0x4000)
         return SIG::noteLookup_7(note_7_);

      note_7_ -= 0x80 * 12;
      return SIG::noteLookup_7(note_7_) << 1;
   }

   void updateFreq()
   {
      note_slew = (note << 7) | detune7;
   }

   Wave     wave{SINE};
   uint8_t  note{};       //!< MIDI note number (can extend above 127)
   uint8_t  detune7{};    //!< MIDI note fraction (7 bit)
   uint8_t  amp_index{};

   SIG::Osc::Sine     sine{};
   SIG::Osc::Triangle triangle{};
   SIG::Osc::Ramp     ramp{};
   SIG::Osc::Square   square{};
   SIG::Osc::Pwm      pulse{};
   SIG::Osc::Square   phase_ref{};
   SIG::Osc::Noise    noise{};
   SIG::ExpSlew       gain{0.1f};
   SIG::ExpSlew       note_slew{0.1f};

   SIG::Osc::Base* osc[6] =
   {
      &sine,
      &triangle,
      &ramp,
      &square,
      &pulse,
      &phase_ref
   };
};
