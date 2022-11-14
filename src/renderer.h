#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

typedef enum { SHADER_NONE=-1, SHADER_VERTEX, SHADER_FRAGMENT } ShaderType;

typedef struct {
  
  std::string vertex_source;
  std::string fragment_source;

} ShaderSource;

