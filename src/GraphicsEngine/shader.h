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

enum ShaderStyle { SHADER_WORLDSPACE, SHADER_VIEWSPACE, SHADER_LIGHTSOURCE };
enum ShaderType { SHADER_NONE=-1, SHADER_VERTEX, SHADER_FRAGMENT };

struct ShaderSource {
  std::string vertex_source;
  std::string fragment_source;
};

ShaderSource parse_shader(const std::string &vertex_shader, const std::string &fragment_shader);
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