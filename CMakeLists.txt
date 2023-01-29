cmake_minimum_required(VERSION 3.24)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)



set(COOM_DEBUG TRUE)
set(COOM_SHIPPING FALSE)


add_compile_options( -pedantic -Wextra )

if (COOM_DEBUG)
  add_compile_options( -g )
  add_definitions( -DCOOM_DEBUG )
else()
  add_compile_options( -Ofast )
endif()

if (COOM_SHIPPING)
  add_definitions( -DCOOM_SHIPPING )
else()

endif()

set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17 -fdiagnostics-color=always")

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
  "src/GraphicsEngine/transform/*.cpp"
  "src/GraphicsEngine/primitive/*.cpp"
  "src/GraphicsEngine/model/*.cpp"
  "src/GraphicsEngine/model/animation/*.cpp"
  "src/GraphicsEngine/renderer/*.cpp"
  
  "src/GameEngine/*.cpp"
  "src/GameEngine/computemesh/*.cpp"
  "src/GameEngine/gameobject/*.cpp"
  "src/GameEngine/player/*.cpp"
  "src/GameEngine/scenegraph/*.cpp"
  "src/GameEngine/scenegraph/fileio/*.cpp"

  "src/EditorUI/*.cpp"
  "src/EditorUI/tabs/*.cpp"

  "src/ui/*.cpp"
  "src/ui/tabs/*.cpp"
  "src/ui/tabs/properties/*.cpp"
  "src/ui/tabs/gameobjects/*.cpp"

  "src/scene/*.cpp"
  "src/scripting/*.cpp"
  "src/scripting/library/*.cpp"

  "src/audio/*.cpp"

  "src/include/stb_image/*.cpp"
  "src/include/imgui/*.cpp"
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


