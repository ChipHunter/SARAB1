cmake_minimum_required (VERSION 3.7.2)
project (SARAB1)

# cross-compiling configs
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

# include external projects
# sql-parser
include(ExternalProject)
ExternalProject_Add(sql-parser
  DOWNLOAD_DIR      ${CMAKE_SOURCE_DIR}/third/sql_parser
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/third/sql_parser
  GIT_REPOSITORY    https://github.com/hyrise/sql-parser.git
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     make CXX=${CMAKE_CXX_COMPILER} -C <SOURCE_DIR> 
  INSTALL_COMMAND   ""
)

#Bring the headers into the project
include_directories(plat/GPIO/include)
include_directories(plat/SPI/include)
include_directories(plat/I2C/include)
include_directories(plat/os_wrappers/include)
 
# Add source files 
file(GLOB SOURCES plat/GPIO/src/*.cpp ${SOURCES})
file(GLOB SOURCES plat/SPI/src/*.cpp ${SOURCES})
file(GLOB SOURCES plat/I2C/src/*.cpp ${SOURCES})
file(GLOB SOURCES plat/os_wrappers/src/*.cpp ${SOURCES})
file(GLOB SOURCES prod/*.cpp ${SOURCES})

add_executable(sarab1 ${SOURCES})
