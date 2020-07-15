#!/bin/bash

init() {

  echo "############### INITIALIZATION ###############"
  # mount the target using nfs
  #sudo mount -t nfs 192.168.1.107:/ /media/nfs
  # define some env variables 
  export CXX=/usr/bin/arm-linux-gnueabihf-g++
  export CC=/usr/bin/arm-linux-gnueabihf-gcc
  export SYSROOT=/media/nfs
  export EXE_NAME="sarab1"

  # define some dirs
  LIB_DIR="${SYSROOT}/usr/local/lib"
  SRC_DIR=$(pwd)
  BUILD_DIR="$(pwd)/build"
  LIBRTA_DIR="src/third/librta/src"
  EXE_INSTALL_DIR="${SYSROOT}/tmp"
  
  # create build dir
  mkdir -p build

}

build_librta() {

  echo "########### BUILDING LIBRTA #############"
  cd ${LIBRTA_DIR}
  make CXX=${CC}

  cd  ${SRC_DIR}
}

clean_librta() {

  echo "########### CLEANING LIBRTA #############"
  cd ${LIBRTA_DIR}
  make clean
  cd  ${SRC_DIR}

}

build_main() {

  echo "############### BUILDING MAIN ###############"
  cd ${BUILD_DIR}
  cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake ../src
  cmake -G "Unix Makefiles" ../src
  cmake -DCMAKE_BUILD_TYPE=Debug ../src
  cmake --build . --config Debug --target all
  echo "############### COPYING EXEs ################"
  if [ $? -eq 0 ]; then
    cp "${BUILD_DIR}/${EXE_NAME}" ${EXE_INSTALL_DIR} 
    cp "${SRC_DIR}/configs/easyloggingpp.conf" ${EXE_INSTALL_DIR}
  fi

  cd  ${SRC_DIR}

}

clean_main() {

  cd ${BUILD_DIR}
  make clean
  cd  ${SRC_DIR}

}

build() {

  echo "################# BUILDING ##################"
  init
  build_librta
  build_main

}

clean() {

  echo "################# CLEANING ##################"
  init
  clean_librta
  clean_main

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
