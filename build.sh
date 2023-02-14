#!/bin/bash

# Build project
mkdir build output
cd build
cmake -G Ninja ..
ninja


# Copy game files to output folder
cd ..

cp ./build/IDKEngine ./output/IDKEngine
cp -r ./assets ./output/
cp ./imgui.ini ./output/imgui.ini

cp -r ./LuaScripting ./output/
