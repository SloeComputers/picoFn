#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# pylint: disable=bad-indentation

import math
import table

import Table_consts as C

table.gen('ramp_dn',
          func      = lambda i,x : int((1 - 2 * x) * C.SAMPLE_MAX + 0.5),
          typename  = C.SAMPLE_TYPE,
          log2_size = C.LOG2_WAVE_SIZE,
          fmt       = C.SAMPLE_FORMAT)
