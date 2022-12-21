#!/bin/bash
mkdir build output
cd build
cmake ..
make

# Copy game files to output folder
cd ..

cp ./game ./output/game
cp ./assets ./output/assets
cp ./src/LuaScripting ./output/src/LuaScripting