#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

enum ShaderType {
  SHADER_WORLDSPACE,
  SHADER_WEAPON,
  SHADER_LIGHTSOURCE,
  SHADER_SHADOWMAP,
  SHADER_POINTSHADOW,
  SHADER_DIRSHADOW,
  SHADER_SCREENQUAD,
  SHADER_TERRAIN,
  SHADER_NORMALS,
  SHADER_VOLUMETRIC_LIGHT,
  SHADER_BLUR
};

struct ShaderSource {
  std::string vertex_source;
  std::string fragment_source;
  std::string geometry_source;
};

ShaderSource parse_shader(const std::string &vertex_shader, const std::string &fragment_shader, const std::string &geometry_shader);
unsigned int compile_shader(unsigned int type, const std::string &source);
unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader, const std::string &geometry_shader);


class Shader {
  private:

  public:
    GLuint id;
    Shader() { };
    void set(GLuint shader_id) { this->id = shader_id; };
    GLuint get(void) { return this->id; };
    void setVec3(const char *uniform_name, glm::vec3 vec);
    void setMat4(const char *uniform_name, glm::mat4 mat);
    void setInt(const char *uniform_name, GLuint value);
    void setFloat(const char *uniform_name, float value);
    void setFloatVector(const char *uniform_name, int size, float *ptr);
};
