
cmake_minimum_required(VERSION 3.24)
project(pit-smellers)

include_directories(
  external/glew/include/
)

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  GLEW
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
  "src/GraphicsEngine/*.cpp"
  "src/GameEngine/*.cpp"
)

find_package(SDL2 REQUIRED)
# find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)


add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
  ${SDL2_LIBRARY}
)

target_link_libraries(${PROJECT_NAME}
  ${ALL_LIBS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
# target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})












# cmake_minimum_required(VERSION 3.24)
# project(pit-smellers)


# file(
#   GLOB SOURCES
#   "src/*.cpp"
#   "src/include/stb_image/*.cpp"
#   "src/include/glew/include"
#   "src/include/SLD2/include"
#   "src/GraphicsEngine/*.cpp"
#   "src/GameEngine/*.cpp"
# )

# find_package(SDL2 REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(OpenGL REQUIRED)


# add_executable(
#   ${PROJECT_NAME}
#   ${SOURCES}
#   ${SDL2_LIBRARY}
# )

# target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
# target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
# target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

# target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})