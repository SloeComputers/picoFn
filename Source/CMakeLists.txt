#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

autogen_py(Table_freq_mHz)
autogen_py(Table_atten_cB)

if("${PDK_TARGET}" STREQUAL "vst3")

   smtg_enable_vst3_sdk()

   smtg_add_vst3plugin(picoFn
                       picofn_plugin.cpp)

   target_include_directories(picoFn PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

   target_link_libraries(picoFn PRIVATE PLT TGT_PLG)

   if(SMTG_MAC)
      smtg_target_set_bundle(picoFn
         BUNDLE_IDENTIFIER com.local.picoFn
         COMPANY_NAME "Sloe Computers")

      set_target_properties(picoX7 PROPERTIES
         MACOSX_BUNDLE_INFO_PLIST           "${CMAKE_CURRENT_LIST_DIR}/../PDK/TGT/vst3/MacVST3Info.plist.in"
         MACOSX_BUNDLE_BUNDLE_NAME          "${CMAKE_PROJECT_NAME}"
         MACOSX_BUNDLE_GUI_IDENTIFIER       "com.local.picoFn"
         MACOSX_BUNDLE_BUNDLE_VERSION       "${PROJECT_VERSION}"
         MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
         MACOSX_BUNDLE_LONG_VERSION_STRING  "${PROJECT_VERSION}"
         MACOSX_BUNDLE_INFO_STRING          "${PROJECT_DESCRIPTION}"
         MACOSX_BUNDLE_COPYRIGHT            "Copyright (c) 2026 John D. Haughton")

      add_custom_command(TARGET picoFn POST_BUILD
         COMMAND /usr/bin/codesign --force --sign - --deep "$<TARGET_BUNDLE_DIR:picoX7>"
         COMMENT "Ad-hoc signing VST3 bundle")
   endif()

else()

   add_picoX7_executable(picoFn
                            picofn.cpp
                            symbol.cpp
                            Table_freq_mHz.cpp
                            Table_atten_cB.cpp
                         LIBRARIES
                            SIG_FL32)

endif()
