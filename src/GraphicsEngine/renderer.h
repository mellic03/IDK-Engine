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

#include "camera.h"

#define SCR_WDTH 1920
#define SCR_HGHT 1080

#define RENDER_DISTANCE 10000.0f
#define NEAR_PLANE_DIST 0.1f

extern float fov;

extern Camera main_cam;

typedef enum { SHADER_NONE=-1, SHADER_VERTEX, SHADER_FRAGMENT } ShaderType;

typedef struct {
  
  std::string vertex_source;
  std::string fragment_source;

} ShaderSource;

void engine_input(SDL_Event *event);



ShaderSource parse_shader(const std::string &filepath);
unsigned int compile_shader(unsigned int type, const std::string &source);
unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader);




class LightSource {

  public:
    glm::vec4 color = {0, 0, 0, 1};
    glm::vec3 position = {0, 0, 0};



};
