#####
# 'TeensyFSW' Deployment:
#
# This registers the 'TeensyFSW' deployment to the build system. 
# Custom components that have not been added at the project-level should be added to 
# the list below.
#
#####

restrict_platforms(ArduinoFw)

###
# Topology and Components
###
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Top/")

# Add custom components to this specific deployment here
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/PDU/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Sensors/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Heater/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ModeManager/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/TlmDispatcher/")

set(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Main.cpp")
set(MOD_DEPS ${FPRIME_CURRENT_MODULE}/Top)

register_fprime_deployment()

finalize_arduino_executable("${FPRIME_CURRENT_MODULE}")

