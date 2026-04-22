#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(picoFn
        LANGUAGES CXX C ASM
        VERSION 0.04)

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Build unit tests")
endif()
include(CTest)

set(HWR_CONFIG picoX7)

set(SIG_SAMPLE_RATE "96000" CACHE STRING "SIG library sample rate (Hz)")

add_subdirectory(PDK)
add_subdirectory(Source)
