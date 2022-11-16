g++ \
./src/include/stb_image/stb_image.cpp \
\
./src/GraphicsEngine/renderer.cpp \
./src/GraphicsEngine/model.cpp \
./src/GraphicsEngine/camera.cpp \
\
./src/GameEngine/player.cpp \
./src/GameEngine/gameobject.cpp \
\
./src/main.cpp \
\
-o main -lSDL2 -lGL -lGLEW -lm -g