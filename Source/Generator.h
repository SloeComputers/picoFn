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

   int16_t left()  { return int16_t(osc_lft() * 0x7fff); }
   int16_t right() { return int16_t(osc_rgt() * 0x7fff); }

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
      // 1st col, AKAI MPKmini, 2nd col, AKAI MIDImix
      switch(control_)
      {
      case 2: case 16: osc_lft.noteOn(value_ + 8); break; // K1  #1 TOP
      case 3: case 17: osc_lft.setDetune(value_);  break; // K2  #1 MID
      case 4: case 18: osc_lft.setPhase(value_);   break; // K3  #1 BTM
      case 5: case 19: osc_lft.setAmpl(value_);    break; // K4  #1 SLIDER

      case 6: case 20: osc_rgt.noteOn(value_ + 8); break; // K5  #2 TOP
      case 7: case 21: osc_rgt.setDetune(value_);  break; // K6  #2 MID
      case 8: case 22: osc_rgt.setPhase(value_);   break; // K7  #2 BTM
      case 9: case 23: osc_rgt.setAmpl(value_);    break; // K8  #2 SLIDER
      }
   }

   void voiceProgram(unsigned, uint8_t prog_) override
   {
      // AKAI MPKmini
      switch(prog_)
      {
      case 0: osc_rgt.changeWave(-1); break; // PAD 1
      case 1: osc_rgt.changeWave(+1); break; // PAD 2
      case 4: osc_lft.changeWave(-1); break; // PAD 5
      case 5: osc_lft.changeWave(+1); break; // PAD 6
      }
   }

   void voiceOn(unsigned, uint8_t note_, uint8_t velocity_) override
   {
      // AKAI MIDImix
      switch(note_)
      {
      case 3: osc_lft.changeWave(-1); break; // #1 MUTE
      case 1: osc_lft.changeWave(+1); break; // #1 REC ARM
      case 6: osc_rgt.changeWave(-1); break; // #2 MUTE
      case 4: osc_rgt.changeWave(+1); break; // #2 REC ARM
      }

      if (note_ < 8)
         return;

      Oscillator* osc = left_note ? &osc_lft : &osc_rgt;
      left_note = not left_note;

      osc->noteOn(note_);
      osc->setDetune(0);
      osc->setPhase(0);
   }

   bool       left_note{true};
   Oscillator osc_lft{};
   Oscillator osc_rgt{};
};
