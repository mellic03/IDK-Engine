#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#define SCR_WDTH 1000
#define SCR_HGHT 1000

#define RENDER_DISTANCE 2000.0f


typedef enum { SHADER_NONE=-1, SHADER_VERTEX, SHADER_FRAGMENT } ShaderType;

typedef struct {
  
  std::string vertex_source;
  std::string fragment_source;

} ShaderSource;

void engine_input(SDL_Event *event);



ShaderSource parse_shader(const std::string &filepath);
unsigned int compile_shader(unsigned int type, const std::string &source);
unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader);
