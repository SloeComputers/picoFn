//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MIDI/Instrument.h"
#include "MIDI/Controller.h"

#include "Oscillator.h"

namespace MX = Akai::MIDImix;
namespace MK = Akai::MPKmini;

class Generator : public MIDI::Instrument
{
public:
   Generator()
      : MIDI::Instrument(/* num_voices */ 1, /* base_channel */ 0)
   {
   }

   void sample(int16_t& left_, int16_t& right_)
   {
      SIG::Signal left  = osc_lft(0.0);
      SIG::Signal right = osc_rgt(left);

      left_  = int16_t(left  * 0x7FFF);
      right_ = int16_t(right * 0x7FFF);
   }

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
      case MK::K1: case MX::TOP1: osc_lft.noteOn(value_ + 8); break;
      case MK::K2: case MX::MID1: osc_lft.setDetune(value_);  break;
      case MK::K3: case MX::BTM1: osc_lft.setPhase(value_);   break;
      case MK::K4: case MX::LVL1: osc_lft.setAmpl(value_);    break;

      case MK::K5: case MX::TOP2: osc_rgt.noteOn(value_ + 8); break;
      case MK::K6: case MX::MID2: osc_rgt.setDetune(value_);  break;
      case MK::K7: case MX::BTM2: osc_rgt.setPhase(value_);   break;
      case MK::K8: case MX::LVL2: osc_rgt.setAmpl(value_);    break;
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
      case 3:  osc_lft.changeWave(-1); break; // #1 MUTE
      case 1:  osc_lft.changeWave(+1); break; // #1 REC ARM
      case 6:  osc_rgt.changeWave(-1); break; // #2 MUTE
      case 4:  osc_rgt.changeWave(+1); break; // #2 REC ARM

      case 7:  osc_rgt.changeMod(+1);  break; // #3 MUTE
      case 9:  osc_rgt.changeMod(-1);  break; // #3 REC ARM

      case 10: osc_lft.toggleLFO();    break; // #4 MUTE
      }

      if (note_ <= 12)
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
