#!/bin/bash

init() {

  echo "############### INITIALIZATION ###############"
  # define some env variables 
  export CC=/usr/bin/arm-linux-gnueabihf-g++
  export SYSROOT=/media/nfs
  export EXE_NAME="sarab1"

  # define some dirs
  LIB_DIR="${SYSROOT}/usr/local/lib"
  SRC_DIR=$(pwd)
  BUILD_DIR="$(pwd)/build"
  SQLPARSER_DIR="src/third/sqlparser"
  EXE_INSTALL_DIR="${SYSROOT}/tmp"
  
  # create build dir
  mkdir -p build

}

build_sqlparser() {

  echo "########### BUILDING SQLPARSER #############"
  cd ${SQLPARSER_DIR}
  make CXX=${CC}

  if [ $? -eq 0 ]; then
    cp libsqlparser.so ${LIB_DIR} 
  fi

}

build_main() {

  echo "############### BUILDING MAIN ###############"
  cd ${BUILD_DIR}
  cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake ../src
  cmake -G "Unix Makefiles" ../src
  cmake --build . --config Debug --target all

  echo "############### COPYING EXEs ################"
  if [ $? -eq 0 ]; then
    cp "${BUILD_DIR}/${EXE_NAME}" ${EXE_INSTALL_DIR} 
    cp "${SRC_DIR}/configs/easyloggingpp.conf" ${EXE_INSTALL_DIR}
  fi

}

build() {

  echo "################# BUILDING ##################"
  init
  build_sqlparser
  build_main

}

clean() {

  echo "################# CLEANING ##################"
  init
  cd ${SQLPARSER_DIR}
  echo ${SQLPARSER_DIR}
  make clean
  cd ${BUILD_DIR}
  make clean

}

clean_all() {

  clean
  rm -rf ${BUILD_DIR}

}

if [ "$1" = "clean" ]; then
  clean
elif [ "$1" = "clean_all" ]; then
  clean_all
else
  build
fi
