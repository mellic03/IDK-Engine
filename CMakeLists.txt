cmake_minimum_required(VERSION 3.24)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

add_compile_options(
  -g
)

set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

project(game)


add_subdirectory (external)
include_directories(
  external/glew-2.1.0/include/
  external/luacpp/
)

set(ALL_LIBS
  ${OPENGL_LIBRARY}
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
  "src/include/stb_image/*.cpp"
  "src/include/imgui/*.cpp"
  "src/GraphicsEngine/*.cpp"
  "src/GraphicsEngine/model/*.cpp"
  "src/GameEngine/*.cpp"
  "src/GameEngine/computemesh/*.cpp"
  "src/ui/*.cpp"
  "src/scene/*.cpp"
  "src/LuaScripting/*.cpp"

  "external/luacpp/*.cpp"
  "external/luacpp/Engine/*.cpp"
  "external/luacpp/Registry/*.cpp"
)


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua51 REQUIRED)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
  ${SDL2_LIBRARY}
)


target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
  ${ALL_LIBS}
)

target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})


set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
