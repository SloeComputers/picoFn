#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

import Table_consts as C

table.gen('atten_cB',
          func      = lambda i,x : int((-20 * math.log10(C.amp_fn(x)) * 10) + 0.5) if i > 0 else C.ATTEN_INFINITY,
          typename  = "uint16_t",
          log2_size = 7,
          fmt       = table.dec_fmt(16))
