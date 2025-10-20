#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

import Table_consts as C

table.gen('freq_mHz',
          func      = lambda i,x : int(C.index2freq(i) * 1000 + 0.5),
          typename  = "uint32_t",
          log2_size = C.FREQ_BITS,
          fmt       = '8d')
