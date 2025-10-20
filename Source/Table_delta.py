#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

import Table_consts as C

#-------------------------------------------------------------------------------
# Frequency tables

def delta_fn(i, _):
   """ Convert table index to a .n-bit frequency ratio """
   ratio = C.index2freq(i) / C.DAC_FREQ
   return int(ratio * (1 << C.PHASE_BITS) + 0.5)

table.gen('delta',
          func      = delta_fn,
          typename  = C.PHASE_TYPE,
          log2_size = C.FREQ_BITS,
          fmt       = table.hex_fmt(C.PHASE_BITS))
