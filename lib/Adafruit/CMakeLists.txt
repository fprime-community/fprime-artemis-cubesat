
####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
# Note: using PROJECT_NAME as EXECUTABLE_NAME
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Adafruit_BusIO/Adafruit_BusIO_Register.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Adafruit_BusIO/Adafruit_I2CDevice.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Adafruit_BusIO/Adafruit_SPIDevice.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Adafruit_Sensor/Adafruit_Sensor.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Adafruit_LSM6DS/Adafruit_LSM6DSOX.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Adafruit_LSM6DS/Adafruit_LSM6DS.cpp"
)

register_fprime_module()
### UTs ###
#set(UT_SOURCE_FILES
#  "${CMAKE_CURRENT_LIST_DIR}/CmdDispatcher.fpp"
#  "${CMAKE_CURRENT_LIST_DIR}/test/ut/CommandDispatcherTester.cpp"
#  "${CMAKE_CURRENT_LIST_DIR}/test/ut/CommandDispatcherImplTester.cpp"
#)
#register_fprime_ut()
