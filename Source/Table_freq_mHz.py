#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

A4_FREQ              = 440   # freq of the A above middle C (Hz)
SEMITONES_PER_OCTAVE = 12    # Size of a musical octave in semitones
FREQ_FRAC_BITS       = 7
FREQ_BITS            = 7 + FREQ_FRAC_BITS
A4_SEMITONE_INDEX    = 61    # Note index for the A above middle C (XXX not MIDI)

def index2freq(index):
   """ Convert table index to freq (Hz) """
   scale = 1 << FREQ_FRAC_BITS
   exp   = (index - (A4_SEMITONE_INDEX * scale)) / (SEMITONES_PER_OCTAVE * scale)
   freq  = A4_FREQ * math.pow(2, exp)
   return freq

table.gen('freq_mHz',
          func      = lambda i,x : int(index2freq(i) * 1000 + 0.5),
          typename  = "uint32_t",
          log2_size = FREQ_BITS,
          fmt       = '8d')
