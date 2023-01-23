#!/bin/bash

# Build project
mkdir build output
cd build
cmake -G Ninja ..
ninja


# Copy game files to output folder
cd ..

cp ./build/CoomEngine ./output/CoomEngine
cp -r ./assets ./output/
cp ./imgui.ini ./output/imgui.ini

cp -r ./LuaScripting ./output/
