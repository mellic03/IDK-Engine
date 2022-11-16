g++ \
./src/main.cpp \
./src/renderer.cpp ./src/model.cpp ./src/camera.cpp \
./include/stb_image/stb_image.cpp \
-o main -lSDL2 -lGL -lGLEW -lm -g