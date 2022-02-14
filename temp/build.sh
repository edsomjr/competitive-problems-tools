#!/bin/bash
g++ -o init.so -fPIC -shared init.cpp
g++ -o default.so -fPIC -shared default.cpp

g++ -std=c++17 -o prog main.cpp pluginmanager.cpp -ldl

mv *.so ./plugins/

export LD_LIBRARY_PATH=.
# ./prog
