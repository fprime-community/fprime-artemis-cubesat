####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
# Note: using PROJECT_NAME as EXECUTABLE_NAME
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/LSM6DS.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/LSM6DS.cpp"
)
target_use_arduino_libraries("Adafruit_LSM6DS")

register_fprime_module()
