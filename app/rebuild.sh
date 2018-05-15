#!/bin/sh

rm -rf build
mkdir build
cd build
cmake -DBOARD=nucleo_f411re ..
cd ..
./run.sh
