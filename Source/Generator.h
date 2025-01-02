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

#pragma once

#include "STB/MIDIInstrument.h"

#include "Oscillator.h"

class Generator : public MIDI::Instrument</* NUM_VOICES */ 2>
{
public:
   Generator() = default;

   int16_t left()  { return osc_lft(); }
   int16_t right() { return osc_rgt(); }

   void getInfo(unsigned voice_, char* buffer_) 
   {
      Oscillator* osc = getOsc(voice_);

      // Frequency
      char     freq_text[16];
      unsigned freqx1000 = osc->getFreqx1000();
      unsigned units     = freqx1000 / 1000;
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
      unsigned note   = osc->getNote();
      unsigned detune = osc->getDetune();
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
         snprintf(note_text, 16, "---");
      }

      // Amplitude
      char     ampl_text[16];
      unsigned ampl = osc->getAmpl() * 99 / 127;
      snprintf(ampl_text, 16, "%2u", ampl);

      snprintf(buffer_, 16, "f%6s %3s a%2s", freq_text, note_text, ampl_text);
   }

private:
   void voiceControl(unsigned voice_, uint8_t control_, uint8_t value_) override
   {
      if (voice_ != 0) return;

      switch(control_)
      {
      case 2: osc_lft.setNote(value_);   break;
      case 3: osc_lft.setDetune(value_); break;
      case 4: osc_lft.setPhase(value_);  break;
      case 5: osc_lft.setAmpl(value_);   break;

      case 6: osc_rgt.setNote(value_);   break;
      case 7: osc_rgt.setDetune(value_); break;
      case 8: osc_rgt.setPhase(value_);  break;
      case 9: osc_rgt.setAmpl(value_);   break;
      }
   }

   Oscillator* getOsc(unsigned voice_)
   {
      switch(voice_)
      {
      case 0: return &osc_lft; break;
      case 1: return &osc_rgt; break;
      }

      return nullptr;
   }

   void voiceOn(unsigned voice_, uint8_t note_, uint8_t velocity_) override
   {
      Oscillator* osc = getOsc(voice_);

      osc->setNote(note_ - 12);
      osc->setDetune(0);
      osc->setPhase(0);
      osc->setAmpl(velocity_);
   }

   Oscillator osc_lft{};
   Oscillator osc_rgt{};
};

