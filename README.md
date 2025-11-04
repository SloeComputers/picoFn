# picoFn

MIDI controlled audio function generator.

## Features

+ Full MIDI control
+ Two channels
+ Sine, triangle, square, pulse, ramp-up, ramp-down, noise waveforms
+ Frequency range 12.97Hz (G0#) to 15.80KHz (B9)
+ Coarse tune with semitone resolution and fine tune with semitone range
+ Phase shift
+ Attenuation 0dB to 59.5dB and infinity

## MIDI controls

| MIDI chan | MIDI type | MIDI Value | Output | Description |
|-----------|-----------|------------|--------|-------------|
| 1 | CTL |  2, 16 | Left | Coarse freqeuncy (semitone) |
| 1 | CTL |  3, 17 | Left | Fine freqeuncy (semitone/128) |
| 1 | CTL |  4, 18 | Left | Phase shift / pulse width | |
| 1 | CTL |  5, 19 | Left | Attenuation (dB) | |
| 1 | PGM |  4 | Left | Waveform select down | |
| 1 | PGM |  5 | Left | Waveform select up | |
| 1 | CTL |  6, 20 | Right | Coarse freqeuncy (semitone) |
| 1 | CTL |  7, 21 | Right | Fine freqeuncy (semitone/128) |
| 1 | CTL |  8, 22 | Right | Phase shift / pulse width | |
| 1 | CTL |  9, 24 | Right | Attenuation (dB) | |
| 1 | PGM |  0 | Right | Waveform select down | |
| 1 | PGM |  1 | Right | Waveform select up | |
| 1 | NON |  note | Left/Right | Coarse frequency for MIDI note value |
| 1 | PGM |  6 | Left+Right | Toggle velocity sense for NON events |

NOTE: These controls have been selected to work well with the AKAI MPKmini or AKAI MIDImix controllers.

## Hardware

Re-uses the [picoX7](https://github.com/AnotherJohnH/picoX7) hardware platform. See [README](https://github.com/AnotherJohnH/Hardware/blob/main/picoX7/README.md)

## Software

### Dependencies

+ https://github.com/AnotherJohnH/Hardware
+ https://github.com/AnotherJohnH/Platform
+ arm-none-eabi-gcc
+ cmake
+ ninja (make works too)

### Checkout

This repo uses git sub-modules, so checkout using --recurse-submodules to clone all the
dependent source...

    git clone --recurse-submodules https://github.com/AnotherJohnH/picoFn.git

or

    git clone --recurse-submodules ssh://git@github.com/AnotherJohnH/picoFn.git

### Build

Being developed on MacOS but should build on Linux too.

    make

Build directly using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rpipico2 -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rpipico2/toolchain.cmake ..
    make

Flashable image will be found under the build sub directory here...

    .../Source/picoFn_I2S_DAC.uf2

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
