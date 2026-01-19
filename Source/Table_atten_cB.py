#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

DB_RANGE       = 60     # range for attenuation control (dB)
ATTEN_INFINITY = 0xFFFF # maximum attenuation

def amp_fn(x):
   return math.pow(10, (DB_RANGE / 20) * (x - 1)) if x > 0 else 0

table.gen('atten_cB',
          func      = lambda i,x : int((-20 * math.log10(amp_fn(x)) * 10) + 0.5) if i > 0 else ATTEN_INFINITY,
          typename  = "uint16_t",
          log2_size = 7,
          fmt       = table.dec_fmt(16))
