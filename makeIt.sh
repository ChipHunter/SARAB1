#!/bin/bash

mkdir -p build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake ../src
cmake -G "Unix Makefiles" ../src
cmake --build . --config Release --target all
