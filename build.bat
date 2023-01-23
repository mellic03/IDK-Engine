mkdir build output
cd build
cmake .. -D CMAKE_C_COMPILER="C:/msys64/mingw64/bin/gcc.exe" -D CMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/g++.exe" -G "Ninja"
ninja


cd ..

copy build\CoomEngine.exe output\CoomEngine.exe
xcopy assets output\assets\ /E /Y
copy imgui.ini output\imgui.ini

xcopy LuaScripting output\LuaScripting\ /E /Y
