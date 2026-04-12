#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_freq_mHz)
autogen_py(Table_atten_cB)

add_picoX7_executable(picoFn
                         picofn.cpp
                         symbol.cpp
                         Table_freq_mHz.cpp
                         Table_atten_cB.cpp
                      LIBRARIES
                         SIG_FP32)
