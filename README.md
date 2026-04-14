# picoFn

2 channel MIDI controlled audio function generator.

## Features

+ Full MIDI control - via MIDI over USB or physical MIDI
+ Two channels
+ Sine, triangle, ramp-up, ramp-down, square, pulse, noise waveforms
+ Sample rate 96 KHz
+ Frequency range 0.1Hz to 20KHz
+ Coarse tune with semitone resolution G-1# to D10
+ Fine tune with semitone range
+ Phase shift / pulse width
+ Attenuation 0dB to 60dB and infinity
+ Modulation/mix of right channel by/with left - Amplitude (AM), Ring (RM), Frequency (FM), Pulse Width (PWM), Mix

## To Do

+ Try some higher sample rates

## MIDI controls

| MIDI chan | MIDI type | MIDI value | Output | Description |
|-----------|-----------|------------|--------|-------------|
| 1 | CTL |  2, 16 | Left | Coarse freqeuncy (semitone) |
| 1 | CTL |  3, 17 | Left | Fine freqeuncy (semitone/128) |
| 1 | CTL |  4, 18 | Left | Phase shift / pulse width | |
| 1 | CTL |  5, 19 | Left | Attenuation (dB) | |
| 1 | PGM |  4 | Left | Waveform select down | |
| 1 | PGM |  5 | Left | Waveform select up | |
| 1 | NON |  1 | Left | Waveform select down | |
| 1 | NON |  3 | Left | Waveform select up | |
| 1 | NON | 10 | Left | Toggle LFO mode | 0.101 Hz - 151.5 Hz |
| 1 | CTL |  6, 20 | Right | Coarse freqeuncy (semitone) |
| 1 | CTL |  7, 21 | Right | Fine freqeuncy (semitone/128) |
| 1 | CTL |  8, 22 | Right | Phase shift / pulse width | |
| 1 | CTL |  9, 24 | Right | Attenuation (dB) | |
| 1 | PGM |  0 | Right | Waveform select down | |
| 1 | PGM |  1 | Right | Waveform select up | |
| 1 | NON |  4 | Right | Waveform select down | |
| 1 | NON |  6 | Right | Waveform select up | |
| 1 | NON |  7 | Right | Modulation select up | |
| 1 | NON |  9 | Right | Modulation select down | |
| 1 | NON |  note | Left/Right | Coarse frequency for MIDI note value |

NOTE: These controls have been selected to work with the AKAI MPKmini or AKAI MIDImix controllers.

## Hardware

Re-uses the [picoX7](https://github.com/SloeComputers/picoX7) hardware platform. See [README](https://github.com/SloeComputers/Hardware/blob/main/picoX7/README.md)

```
                   +-----------+
(Debug) TX UART <- |  1     40 | <> vbus +5v
                   |  2     39 | <> vsys
            gnd ## |  3     38 | ## gnd
                   |  4     37 | <- 3v3-en
                   |  5     36 | -> 3v3
                   |  6     35 | <- adc-vref
                   |  7     34 | -> I2S LRCLK
            gnd ## |  8     33 | ## agnd
                   |  9     32 | -> I2S SCLK
                   | 10     31 | -> I2S SD
                   | 11     30 | <- run
                   | 12     29 |    
            gnd ## | 13     28 | ## gnd
                   | 14     27 | <- UART RX (MIDI in)
                   | 15     26 |    
                   | 16     25 | -> I2C SCL (16x2 LCD)
                   | 17     24 | -> I2C SDA (16x2 LCD)
            gnd ## | 18     23 | ## gnd
                   | 19     22 |    
                   | 20     21 |    
                   +-----------+
```

## Software

### Dependencies

+ https://github.com/SloeComputers/PDK
+ arm-none-eabi-gcc
+ cmake
+ ninja (make works too)

### Checkout

This repo uses git sub-modules, so checkout using --recurse-submodules to clone all the
dependent source...

    git clone --recurse-submodules https://github.com/SloeComputers/picoFn.git

or

    git clone --recurse-submodules ssh://git@github.com/SloeComputers/picoFn.git

### Build

Being developed on MacOS but should build on Linux too.

    make

Build directly using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rp2350 -DCMAKE_TOOLCHAIN_FILE=PDK/MTL/rp2350/target/toolchain.cmake ..
    make

Flashable image will be found under the build sub directory here...

    .../Source/picoFn_RPIPICO_I2S_DAC.uf2

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
