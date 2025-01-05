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

import math
import sys
import Table

def index2freq(index):
   """ Convert 14-bit table index to freq (Hz) """

   A4_SEMITONE_INDEX    = 57     # Note index for the A above middle C
   A4_FREQ              = 440    # freq of the A above middle C (Hz)
   SEMITONES_PER_OCTAVE = 12     # Notes in a musical scale
   FRAC_SEMITONE_BITS   = 7

   scale = 1 << FRAC_SEMITONE_BITS
   exp   = (index - (A4_SEMITONE_INDEX * scale)) / (SEMITONES_PER_OCTAVE * scale)
   freq  = A4_FREQ * math.pow(2, exp)

   return freq

def delta_fn(i, x):
   DAC_FREQ = 48000
   ratio = index2freq(i) / DAC_FREQ
   return int(ratio * (1<<32) + 0.5)

Table.gen('delta',
          bits      = 32,
          func      = delta_fn,
          log2_size = 14,
          typename  = "uint32_t",
          prefix    = '0x',
          fmt       = '08x')

Table.gen('freq',
          bits      = 32,
          func      = lambda i,x : int(index2freq(i) * 1000 + 0.5),
          log2_size = 14,
          typename  = "uint32_t",
          prefix    = '0x',
          fmt       = '08x')

Table.gen('sine',
          bits      = 16,
          func      = lambda i,x : int(math.sin(x * 2 * math.pi) * 0x7FFF + 0.5),
          log2_size = 16,
          typename  = "int16_t",
          prefix    = '',
          fmt       = '6d')

Table.gen('triangle',
          bits      = 16,
          func      = lambda i,x : int(((1 - 4 * x) if x < 0.5 else (4 * x - 3)) * 0x7FFF + 0.5),
          log2_size = 16,
          typename  = "int16_t",
          prefix    = '',
          fmt       = '6d')

Table.gen('ramp_up',
          bits      = 16,
          func      = lambda i,x : int((2 * x - 1) * 0x7FFF + 0.5),
          log2_size = 16,
          typename  = "int16_t",
          prefix    = '',
          fmt       = '6d')

Table.gen('ramp_dn',
          bits      = 16,
          func      = lambda i,x : int((1 - 2 * x) * 0x7FFF + 0.5),
          log2_size = 16,
          typename  = "int16_t",
          prefix    = '',
          fmt       = '6d')
