cmake_minimum_required (VERSION 3.7.2)
project (SARAB1)

set(EXCUTABLE_NAME sarab1)

# set c/c++ version
set(CMAKE_C_STANDARD   99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add excutable
add_executable(${EXCUTABLE_NAME})

# add external projects
include(ExternalProject)
ExternalProject_Add(sqlparser
  GIT_REPOSITORY    https://github.com/hyrise/sql-parser
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/third/sql_parser
  PATCH_COMMAND     patch --forward -d <SOURCE_DIR> -p1 < ${CMAKE_SOURCE_DIR}/../patches/sqlparser.patch
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     make CXX=${CMAKE_CXX_COMPILER} -C <SOURCE_DIR>
  INSTALL_COMMAND   make -C <SOURCE_DIR> install
)
 
ExternalProject_Add(easyloggingpp
  GIT_REPOSITORY    https://github.com/amrayn/easyloggingpp.git
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/third/easyloggingpp
  PATCH_COMMAND     ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     "" 
  INSTALL_COMMAND   ""
)

# define dependencies 
add_dependencies(${EXCUTABLE_NAME} sqlparser)
add_dependencies(${EXCUTABLE_NAME} easyloggingpp)

# add source files 
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/plat/GPIO/src/*.cpp ${SOURCES})
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/plat/SPI/src/*.cpp ${SOURCES})
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/plat/I2C/src/*.cpp ${SOURCES})
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/plat/os_wrappers/src/*.cpp ${SOURCES})
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/prod/*.cpp ${SOURCES})
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/third/easyloggingpp/src/*.cc ${SOURCES})

target_sources(${EXCUTABLE_NAME} PRIVATE ${SOURCES})

# add header locations
target_include_directories(${EXCUTABLE_NAME} PRIVATE plat/GPIO/include)
target_include_directories(${EXCUTABLE_NAME} PRIVATE plat/SPI/include)
target_include_directories(${EXCUTABLE_NAME} PRIVATE plat/I2C/include)
target_include_directories(${EXCUTABLE_NAME} PRIVATE plat/os_wrappers/include)
target_include_directories(${EXCUTABLE_NAME} PRIVATE third/sql_parser/src)
target_include_directories(${EXCUTABLE_NAME} PRIVATE third/easyloggingpp/src)

# add compiler options
target_compile_options(${EXCUTABLE_NAME} PRIVATE -pthread)
target_compile_options(${EXCUTABLE_NAME} PRIVATE -Wall -Werror -Wno-long-long -pedantic)

# find required libs
#find_library(SQLPARSERLIB libsqlparser.so)
# add compile definition
# easyloggingpp
add_compile_definitions(ELPP_THREAD_SAFE)
add_compile_definitions(ELPP_FORCE_USE_STD_THREAD)
add_compile_definitions(ELPP_FEATURE_CRASH_LOG)
# link libs
target_link_libraries(${EXCUTABLE_NAME} PRIVATE ${CMAKE_SYSROOT}/usr/local/lib/libsqlparser.so)
