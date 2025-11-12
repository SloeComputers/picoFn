#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_sine)
autogen_py(Table_triangle)
autogen_py(Table_ramp_up)
autogen_py(Table_ramp_dn)
autogen_py(Table_delta)
autogen_py(Table_freq_mHz)
autogen_py(Table_amp)
autogen_py(Table_atten_cB)

add_picoX7_executable(picoFn
                      picofn.cpp
                      symbol.cpp
                      Table_sine.cpp
                      Table_triangle.cpp
                      Table_ramp_up.cpp
                      Table_ramp_dn.cpp
                      Table_delta.cpp
                      Table_freq_mHz.cpp
                      Table_amp.cpp
                      Table_atten_cB.cpp)
