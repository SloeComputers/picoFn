#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

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
const.decl('DAC_FREQ',          96000)  # Samples per second (Hz)
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

SAMPLE_FORMAT = table.signed_dec_fmt(SAMPLE_BITS)  # For neat layout in the table .cpp files

#-------------------------------------------------------------------------------
# Frequency fns

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

#-------------------------------------------------------------------------------
# Amplitude fns

def amp_fn(x):
   return math.pow(10, (DB_RANGE / 20) * (x - 1)) if x > 0 else 0
