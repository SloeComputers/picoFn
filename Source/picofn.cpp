//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Audio function generator for Raspberry Pi Pico

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoX7/Config.h"
#include "Hardware/FilePortal.h"

#include "Generator.h"
#include "Symbol.h"
#include "Table_const.h"

static const unsigned TICK_RATE        = 400;                   //!< Control rate 400 Hz
static const unsigned SAMPLES_PER_TICK = DAC_FREQ / TICK_RATE;  //!< DAC buffer size (16 bit samples)
static const bool     MIDI_DEBUG       = false;


static hw::FilePortal file_portal{"picoFn",
                                  "https://github.com/AnotherJohnH/picoFn"};
static Generator  generator{};


// --- Physical MIDI -----------------------------------------------------------

static hw::PhysMidi phys_midi{};


// --- USB MIDI ----------------------------------------------------------------

static hw::UsbFileMidi usb{0x91C0, "picoFn", file_portal};

extern "C" void IRQ_USBCTRL() { usb.irq(); }


// --- 16x2 LCD display --------------------------------------------------------

static hw::Lcd lcd{};


// --- LED ---------------------------------------------------------------------

static hw::Led led{};


// --- DAC ---------------------------------------------------------------------

static hw::Audio<SAMPLES_PER_TICK>  audio{DAC_FREQ};

static void hwTick();

#if defined(HW_DAC_I2S)

MTL_AUDIO_ATTACH_IRQ_0(audio);

void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   for(unsigned i = 0; i < n; i++)
   {
      buffer[i] = (generator.left() << 16) | (generator.right() & 0xFFFF);
   }

   hwTick();
}

#elif defined(HW_DAC_PWM)

MTL_AUDIO_ATTACH_IRQ_0(audio);

void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   for(unsigned i = 0; i < n; i++)
   {
      buffer[i] = audio.packSamples(generator.left(), generator.right());
   }

   hwTick();
}

#else

template<>
void hw::Audio<SAMPLES_PER_TICK>::getSamples(int16_t* buffer, unsigned n)
{
   (void) MIDI_DEBUG; // XXX avoid an unrelated warning

   for(unsigned i = 0; i < n; i += 2)
   {
      buffer[i]     = generator.left();
      buffer[i + 1] = generator.right();
   }

   hwTick();
}

#endif


static void hwTick()
{
   phys_midi.tick();
   usb.tick();
}


// --- Entry point -------------------------------------------------------------

int main()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("\n");
   puts(file_portal.addREADME("picoFn"));
   printf("\n");

   phys_midi.setDebug(MIDI_DEBUG);
   phys_midi.attachInstrument(1, generator);

   usb.setDebug(MIDI_DEBUG);
   usb.attachInstrument(1, generator);

   audio.start();

   symbolDefine(lcd);

   while(true)
   {
      for(unsigned line = 0; line < 2; ++line)
      {
         char buffer[17];

         generator.getInfo(/* left */ line == 0, buffer);

         lcd.move(0, line);
         lcd.print(buffer);
      }

      led = generator.isAnyVoiceOn();

      usleep(100000);
   }

   return 0;
}
