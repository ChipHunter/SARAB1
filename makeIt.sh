#!/bin/bash

mkdir -p build
cd build
cmake -G "Unix Makefiles" ../src
cmake --build . --config Release --target all
