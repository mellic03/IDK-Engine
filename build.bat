mkdir build output
cd build
cmake .. -D CMAKE_C_COMPILER="C:/msys64/mingw64/bin/gcc.exe" -D CMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/g++.exe" -G "MinGW Makefiles"
mingw32-make


cd ..

copy build\game.exe output\game.exe
xcopy assets output\assets\ /E /Y
copy config.txt output\config.txt
copy imgui.ini output\imgui.ini

xcopy LuaScripting output\LuaScripting\ /E /Y
