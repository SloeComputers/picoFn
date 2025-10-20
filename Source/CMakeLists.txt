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

foreach(hw_config ${hw_picoX7_config_list})

   add_executable(picoFn_${hw_config}
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

   target_compile_definitions(picoFn_${hw_config} PRIVATE HW_${hw_config})

   target_include_directories(picoFn_${hw_config}
                              PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                              PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

   target_link_libraries(picoFn_${hw_config} PRIVATE HW)

   set_target_properties(picoFn_${hw_config} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endforeach()
