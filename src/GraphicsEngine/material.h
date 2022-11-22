#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

enum ShaderStyle { SHADER_WORLDSPACE, SHADER_VIEWSPACE };

class Texture {

  public:
    std::string m_filename;
    GLenum m_texture_obj;

    Texture() {};

    bool load(const char *filepath);
    void bind(GLenum texture_unit);

};


struct Material {
  glm::vec3 ambient = glm::vec3(0.0f);
  Texture diffuse, specular, emission, normal;
  float spec_exponent = 0.5f;
};

struct Light {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 ambient = glm::vec3(0.3f);
  glm::vec3 diffuse = glm::vec3(1.0f);
  glm::vec3 specular = glm::vec3(1.0f);
};

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