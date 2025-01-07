#-------------------------------------------------------------------------------
#  Copyright (c) 2025 John D. Haughton
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

add_subdirectory(hw)

set(python_path ${CMAKE_CURRENT_SOURCE_DIR}/../Platform/python)

add_custom_command(
   COMMENT
      "Auto-generate function tables"
   COMMAND
      ${CMAKE_COMMAND}
         -E env PYTHONPATH=${python_path}
         ${CMAKE_CURRENT_SOURCE_DIR}/TABLE_GEN.py
   DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/TABLE_GEN.py
      ${python_path}/Table.py
   OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/Table_sine.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_sine.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_triangle.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_triangle.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_ramp_up.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_ramp_up.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_ramp_dn.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_ramp_dn.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_delta.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_delta.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_freq_mHz.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_freq_mHz.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_amp.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_amp.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_atten_cB.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_atten_cB.cpp
   )

foreach(hw_config ${hw_config_list})

   add_executable(picofn_${hw_config}
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

   target_compile_definitions(picofn_${hw_config} PRIVATE HW_${hw_config})
   target_include_directories(picofn_${hw_config}
                              PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                              PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
   target_link_libraries(     picofn_${hw_config} PRIVATE ${hw_target_libs})

endforeach()
