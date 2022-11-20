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

extern float REN_fov;
extern float REN_deltaTime;
extern Camera REN_active_cam;
extern GLuint REN_active_shader;

typedef enum { SHADER_NONE=-1, SHADER_VERTEX, SHADER_FRAGMENT } ShaderType;

typedef struct {
  
  std::string vertex_source;
  std::string fragment_source;

} ShaderSource;


ShaderSource parse_shader(const std::string &filepath);
unsigned int compile_shader(unsigned int type, const std::string &source);
unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader);


class Renderer {
  
  public:
    Camera cam;
    GLuint light_shader;
    GLuint mat_shader;
    float deltaTime = 0.0f;
    float fov = 90.0f;

    Renderer() { };

};

extern Renderer renderer;