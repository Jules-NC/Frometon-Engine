#! /usr/bin/env bash

mkdir -p build
cd build
cmake ..
make
cd src
./imgui-glfw
