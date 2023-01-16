cmake_minimum_required(VERSION 3.24)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

set(COOM_DEBUG_MODE FALSE)



add_compile_options( -pedantic -Wextra )

if (COOM_DEBUG_MODE)
  add_compile_options( -g )
  add_definitions( -DCOOMDEBUG )
else()
  add_compile_options( -Ofast )
endif()

set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

project(CoomEngine)


add_subdirectory (external)
include_directories(
  external/glew-2.1.0/include/
  external/luacpp/
)

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  ${OPENAL_LIBRARY}
  GLEW_210
)

add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
)


file(
  GLOB SOURCES
  "src/*.cpp"

  "src/GraphicsEngine/*.cpp"
  "src/GraphicsEngine/model/*.cpp"
  
  "src/GameEngine/*.cpp"
  "src/GameEngine/computemesh/*.cpp"
  "src/GameEngine/gameobject/*.cpp"
  "src/GameEngine/player/*.cpp"
  "src/GameEngine/scenegraph/*.cpp"

  "src/ui/*.cpp"
  "src/ui/tabs/*.cpp"
  "src/ui/tabs/properties/*.cpp"

  "src/scene/*.cpp"
  "src/scripting/*.cpp"

  "src/audio/*.cpp"

  
  "src/include/stb_image/*.cpp"
  "src/include/imgui/*.cpp"
  "src/include/tiny_gltf/*.h"
  "src/include/tiny_gltf/*.hpp"
  "src/include/tiny_gltf/*.cpp"
)


find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua51 REQUIRED)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
  ${SDL2_LIBRARY}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_mixer_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
  ${ALL_LIBS}
)

target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})


