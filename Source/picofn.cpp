//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Audio function generator for Raspberry Pi Pico

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoX7/Config.h"

#include "Generator.h"
#include "Symbol.h"
#include "Table_const.h"

static const unsigned TICK_RATE        = 400;                   //!< Control rate 400 Hz
static const unsigned SAMPLES_PER_TICK = DAC_FREQ / TICK_RATE;  //!< DAC buffer size (16 bit samples)
static const bool     MIDI_DEBUG       = false;


static Generator                    generator{};
static hw::PhysMidi                 phys_midi{};
static hw::Led                      led{};
static hw::Audio<SAMPLES_PER_TICK>  audio{DAC_FREQ};

#if defined(HW_MIDI_USB_DEVICE)

static hw::MidiUSBDevice midi_usb {generator, 0x91CF, "picoFn", MIDI_DEBUG};

extern "C" void IRQ_USBCTRL() { midi_usb.usb.irq(); }

#endif


static void hwTick()
{
   phys_midi.tick();
#if defined(HW_MIDI_USB_DEVICE)
   midi_usb.tick();
#endif

   led = generator.isAnyVoiceOn();
}


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
   
#elif defined(HW_DAC_NATIVE)

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

int main()
{
   // Clear screen and cursor to home
   printf("\e[2J");
   printf("\e[1,1H");

   printf("\n");
   printf("Program  : picoFn\n");
   printf("Hardware : %s\n", HW_DESCR);
   printf("Author   : Copyright (c) 2025 John D. Haughton\n");
   printf("License  : MIT\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);
   printf("\n");

   audio.start();

   phys_midi.setDebug(MIDI_DEBUG);
   phys_midi.attachInstrument(1, generator);

#if not defined(HW_LCD_NONE)
   hw::Lcd lcd{};
   symbolDefine(lcd);
#endif

   while(true)
   {
      for(unsigned line = 0; line < 2; ++line)
      {
         char buffer[17];

         generator.getInfo(/* left */ line == 0, buffer);
         printf("%s\n", buffer);

#if not defined(HW_LCD_NONE)
         lcd.move(0, line);
         lcd.print(buffer);
#endif
      }

      usleep(100000);
   }

   return 0;
}
