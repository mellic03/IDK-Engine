#!/bin/bash

# Build project
mkdir build output
cd build
cmake ..
make


# Copy game files to output folder
cd ..

cp ./build/game ./output/game
cp -r ./assets ./output/assets
cp ./config.txt ./output/config.txt
cp ./imgui.ini ./output/imgui.ini

cp -r ./LuaScripting ./output/
