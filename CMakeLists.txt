#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.24)

project(picoFn
        LANGUAGES CXX C ASM
        VERSION 0.04)

include(CTest)

set(HWR_CONFIG picoX7)

set(SIG_SAMPLE_RATE "96000" CACHE STRING "SIG library sample rate (Hz)")

add_subdirectory(Platform)
add_subdirectory(Source)
