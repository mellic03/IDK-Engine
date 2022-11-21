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
#include "model.h"

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


class Shader {
  private:
    GLuint id;

  public:
    Shader() { };
    void set(GLuint shader_id) { this->id = shader_id; };
    GLuint get(void) { return this->id; };
    void use(void) { glUseProgram(this->id); };
    void setVec3(const char *uniform_name, glm::vec3 vec);
    void setMat4(const char *uniform_name, glm::mat4 mat);
    void setInt(const char *uniform_name, GLuint value);
    void setFloat(const char *uniform_name, float value);

};

class Renderer {
  
  public:
    Camera cam;
    GLuint light_shader;
    GLuint mat_shader;
    Shader shader;
    Light lightsource;
    float deltaTime = 0.0f;
    float fov = 90.0f;

    int SCR_width = 1500;
    int SCR_height = 900;

    Renderer();

};
extern Renderer renderer;