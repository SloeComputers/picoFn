#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.24)

project(picoFn
        LANGUAGES CXX C ASM
        VERSION 0.02)

include(CTest)

set(HARDWARE picoX7)

add_subdirectory(Hardware)
add_subdirectory(Source)
