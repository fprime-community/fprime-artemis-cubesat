####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/LedBlinker.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/LedBlinker.cpp"
)

# Arduino additions
#if(FPRIME_ARDUINO)
#    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/LedBlinkerArduino.cpp")
#else()
#    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/LedBlinkerLinux.cpp")
#endif()

register_fprime_module()
