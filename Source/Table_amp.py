#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

import Table_consts as C

table.gen('amp',
          func      = lambda i,x : int(C.amp_fn(x) * C.AMP_MAX + 0.5),
          typename  = C.AMP_TYPE,
          log2_size = 7,
          fmt       = table.hex_fmt(C.AMP_BITS))
