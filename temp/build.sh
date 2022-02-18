#!/bin/bash
mkdir -p plugins

g++ -std=c++17 -o init.so -fPIC -shared init.cpp plugin.cpp
g++ -std=c++17 -o default.so -fPIC -shared default.cpp plugin.cpp

g++ -std=c++17 -o prog main.cpp pluginmanager.cpp args.cpp plugin.cpp -ldl

mv *.so ./plugins/

export LD_LIBRARY_PATH=.
# ./prog
