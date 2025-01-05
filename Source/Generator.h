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

class Generator : public MIDI::Instrument</* NUM_VOICES */ 1>
{
public:
   Generator() = default;

   int16_t left()  { return osc_lft(); }
   int16_t right() { return osc_rgt(); }

   void getInfo(bool left_, char* buffer16_) const
   {
      if (left_)
         osc_lft.getInfo(buffer16_);
      else
         osc_rgt.getInfo(buffer16_);

      if (left_note == left_)
         buffer16_[8] = '>';
   }

private:
   void voiceControl(unsigned voice_, uint8_t control_, uint8_t value_) override
   {
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

   void voiceProgram(unsigned, uint8_t prog_) override
   {
      switch(prog_)
      {
      case 0: osc_rgt.changeWave(-1); break;
      case 1: osc_rgt.changeWave(+1); break;

      case 4: osc_lft.changeWave(-1); break;
      case 5: osc_lft.changeWave(+1); break;
      }
   }

   void voiceOn(unsigned, uint8_t note_, uint8_t velocity_) override
   {
      Oscillator* osc = left_note ? &osc_lft : &osc_rgt;
      left_note = not left_note;

      osc->setNote(note_ - 12);
      osc->setDetune(0);
      osc->setPhase(0);
      osc->setAmpl(velocity_);
   }

   bool       left_note{true};
   Oscillator osc_lft{};
   Oscillator osc_rgt{};
};
