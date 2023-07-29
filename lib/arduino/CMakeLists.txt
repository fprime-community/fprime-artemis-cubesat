restrict_platforms(ArduinoFw)

set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/RadioHead/RH_RF22.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RadioHead/RHHardwareSP1I.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RadioHead/RHGenericSPI.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RadioHead/RHGenericDriver.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RadioHead/RHSPIDriver.cpp"
)

add_library(RadioHead ${SOURCE_FILES})
target_include_directories(RadioHead PUBLIC "${CMAKE_CURRENT_LIST_DIR}/RadioHead")
