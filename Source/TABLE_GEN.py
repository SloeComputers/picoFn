#!/usr/bin/env python3
#------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#------------------------------------------------------------------------------
# pylint: disable=bad-indentation

import math
import table

#-------------------------------------------------------------------------------
# Universal constants
A4_FREQ              = 440   # freq of the A above middle C (Hz)
SEMITONES_PER_OCTAVE = 12    # Size of a musical octave in semitones


#-------------------------------------------------------------------------------
# Project constants (also available as globals in this file)

const = table.Constants(globals())

const.decl('A4_SEMITONE_INDEX', 61)     # Note index for the A above middle C (XXX not MIDI)
const.decl('DAC_FREQ',          48000)  # Samples per second (Hz)
const.decl('DB_RANGE',          60)     # Range for attenuation control (dB)
const.decl('SAMPLE_BITS',       16)     # Minimum quality audio DACs support
const.decl('FREQ_BITS',         14)     # Support full control via 2 x MIDI 7-bit values
const.decl('FREQ_FRAC_BITS',    7)      # Support full control via a MIDI 7-bit value
const.decl('PHASE_BITS',        32)     # Size of the phase accumulator
const.decl('LOG2_WAVE_SIZE',    16)     # Wavetable size
const.decl('AMP_BITS',          16)     # Amplitude
const.decl('ATTEN_INFINITY',    0xFFFF) # Maximum attenuation

# derived constants
SAMPLE_TYPE = f'int{SAMPLE_BITS}_t'
PHASE_TYPE  = f'uint{PHASE_BITS}_t'
AMP_TYPE    = f'uint{AMP_BITS}_t'
AMP_MAX     = table.unsigned_max(AMP_BITS)

const.decl('SAMPLE_MAX', table.signed_max(SAMPLE_BITS), SAMPLE_TYPE)

assert FREQ_BITS > FREQ_FRAC_BITS
assert PHASE_BITS >= LOG2_WAVE_SIZE

#-------------------------------------------------------------------------------
# Wavetables

SAMPLE_FORMAT = table.signed_dec_fmt(SAMPLE_BITS)  # For neat layout in the table .cpp files

table.gen('sine',
          func      = lambda i,x : int(math.sin(x * 2 * math.pi) * SAMPLE_MAX + 0.5),
          typename  = SAMPLE_TYPE,
          log2_size = LOG2_WAVE_SIZE,
          fmt       = SAMPLE_FORMAT)

table.gen('triangle',
          func      = lambda i,x : int(((1 - 4 * x) if x < 0.5 else (4 * x - 3)) * SAMPLE_MAX + 0.5),
          typename  = SAMPLE_TYPE,
          log2_size = LOG2_WAVE_SIZE,
          fmt       = SAMPLE_FORMAT)

table.gen('ramp_up',
          func      = lambda i,x : int((2 * x - 1) * SAMPLE_MAX + 0.5),
          typename  = SAMPLE_TYPE,
          log2_size = LOG2_WAVE_SIZE,
          fmt       = SAMPLE_FORMAT)

table.gen('ramp_dn',
          func      = lambda i,x : int((1 - 2 * x) * SAMPLE_MAX + 0.5),
          typename  = SAMPLE_TYPE,
          log2_size = LOG2_WAVE_SIZE,
          fmt       = SAMPLE_FORMAT)


#-------------------------------------------------------------------------------
# Frequency tables

def index2freq(index):
   """ Convert table index to freq (Hz) """
   scale = 1 << FREQ_FRAC_BITS
   exp   = (index - (A4_SEMITONE_INDEX * scale)) / (SEMITONES_PER_OCTAVE * scale)
   freq  = A4_FREQ * math.pow(2, exp)
   return freq

def delta_fn(i, _):
   """ Convert table index to a .n-bit frequency ratio """
   ratio = index2freq(i) / DAC_FREQ
   return int(ratio * (1 << PHASE_BITS) + 0.5)

table.gen('delta',
          func      = delta_fn,
          typename  = PHASE_TYPE,
          log2_size = FREQ_BITS,
          fmt       = table.hex_fmt(PHASE_BITS))

table.gen('freq_mHz',
          func      = lambda i,x : int(index2freq(i) * 1000 + 0.5),
          typename  = "uint32_t",
          log2_size = FREQ_BITS,
          fmt       = '8d')


#-------------------------------------------------------------------------------
# Amplitude attenuation tables

def amp_fn(x):
   return math.pow(10, (DB_RANGE / 20) * (x - 1)) if x > 0 else 0

table.gen('amp',
          func      = lambda i,x : int(amp_fn(x) * AMP_MAX + 0.5),
          typename  = AMP_TYPE,
          log2_size = 7,
          fmt       = table.hex_fmt(AMP_BITS))

table.gen('atten_cB',
          func      = lambda i,x : int((-20 * math.log10(amp_fn(x)) * 10) + 0.5) if i > 0 else ATTEN_INFINITY,
          typename  = "uint16_t",
          log2_size = 7,
          fmt       = table.dec_fmt(16))
