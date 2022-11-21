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

#include "material.h"

#define RENDER_DISTANCE 10000.0f
#define NEAR_PLANE_DIST 0.1f

enum ShaderType { SHADER_NONE=-1, SHADER_VERTEX, SHADER_FRAGMENT };

struct ShaderSource {
  std::string vertex_source;
  std::string fragment_source;
};

ShaderSource parse_shader(const std::string &filepath);
unsigned int compile_shader(unsigned int type, const std::string &source);
unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader);


class Renderer {
  
  public:
    GLuint light_shader;
    GLuint mat_shader;
    Shader shader;
    Shader shaders[1];
    Light lightsource;
    float deltaTime = 0.0f;
    float fov = 90.0f;

    int SCR_width = 1500;
    int SCR_height = 900;

    Renderer();

};