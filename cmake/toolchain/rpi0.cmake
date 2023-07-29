SET(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_C_COMPILER   /opt/cross-pi-gcc-pizero/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /opt/cross-pi-gcc-pizero/bin/arm-linux-gnueabihf-g++)

SET(CMAKE_FIND_ROOT_PATH  /opt/cross-pi-gcc-pizero/arm-linux-gnueabihf)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)