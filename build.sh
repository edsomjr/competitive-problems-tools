#!/bin/bash

mkdir -p build
cd build
cmake .. 
make 
cpack
cp src/ejproblem2 ../src
cp ejproblem-* ..
cd ..
