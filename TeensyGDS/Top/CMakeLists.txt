####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
####

set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/instances.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/TeensyGDSPackets.xml"
  "${CMAKE_CURRENT_LIST_DIR}/topology.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/TeensyGDSTopology.cpp"
)
set(MOD_DEPS
  Fw/Logger
  Arduino/ArduinoTime
  Arduino/Drv/StreamDriver
  Os/Baremetal/TaskRunner
)

register_fprime_module()
