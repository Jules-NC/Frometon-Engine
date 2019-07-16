#! /usr/bin/env bash

mkdir -p lib
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -P src/ -nc

wget https://anim1-s12.wp.rpi.edu/files/2012/03/TextureGrid.jpg -P res/ -nc
 

mkdir -p build
cd build
cmake ..
make
cd src
./Frometon-Engine

