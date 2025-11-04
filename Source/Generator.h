//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "STB/MIDIInstrument.h"

#include "Oscillator.h"

class Generator : public MIDI::Instrument
{
public:
   Generator()
      : MIDI::Instrument(/* num_voices */ 1, /* base_channel */ 0)
   {
   }

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
      case 2: case 16: osc_lft.setNote(value_);   break;
      case 3: case 17: osc_lft.setDetune(value_); break;
      case 4: case 18: osc_lft.setPhase(value_);  break;
      case 5: case 19: osc_lft.setAmpl(value_);   break;

      case 6: case 20: osc_rgt.setNote(value_);   break;
      case 7: case 21: osc_rgt.setDetune(value_); break;
      case 8: case 22: osc_rgt.setPhase(value_);  break;
      case 9: case 23: osc_rgt.setAmpl(value_);   break;
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
      case 6: touch_sense = not touch_sense; break;
      }
   }

   void voiceOn(unsigned, uint8_t note_, uint8_t velocity_) override
   {
      if (note_ < 8)
         return;

      Oscillator* osc = left_note ? &osc_lft : &osc_rgt;
      left_note = not left_note;

      osc->setNote(note_ - 8);
      osc->setDetune(0);
      osc->setPhase(0);

      if (touch_sense)
         osc->setAmpl(velocity_);
   }

   bool       left_note{true};
   bool       touch_sense{true};
   Oscillator osc_lft{};
   Oscillator osc_rgt{};
};
