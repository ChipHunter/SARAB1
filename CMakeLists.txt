cmake_minimum_required (VERSION 3.7.2)
project (ROBO1)

# cross-compiling configs
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

#set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)
#set(CMAKE_STAGING_PREFIX /home/devel/stage)

set(tools /home/ammar/devel/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf)
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# set c/c++ version
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# add compiler flags
set(GCC_PTHREAD_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_PTHREAD_FLAGS}")

#Bring the headers into the project
include_directories(comp/GPIO/include)
include_directories(comp/SPI/include)
include_directories(comp/I2C/include)
 
# Add source files 
file(GLOB SOURCES comp/GPIO/src/*.cpp ${SOURCES})
file(GLOB SOURCES comp/SPI/src/*.cpp ${SOURCES})
file(GLOB SOURCES comp/I2C/src/*.cpp ${SOURCES})
file(GLOB SOURCES prod/*.cpp ${SOURCES})

add_executable(robo1 ${SOURCES})
