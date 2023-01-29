#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <functional>

#include <GL/glew.h>
#include <GL/glu.h>

#include "glcall.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

enum ShaderType {
  SHADER_BACKGROUND,
  SHADER_TERRAIN,
  SHADER_BILLBOARD_FIXED,
  SHADER_BILLBOARD_FOLLOW,
  SHADER_ACTOR,
  SHADER_ACTOR_ANIMATED,
  SHADER_LIGHTSOURCE,
  SHADER_POINTSHADOW,
  SHADER_DIRSHADOW,
  SHADER_SCREENQUAD,
  SHADER_VOLUMETRIC_LIGHT,
  SHADER_BLUR_DOWNSAMPLE,
  SHADER_BLUR_UPSAMPLE,
  SHADER_ADDITIVE,
  SHADER_GBUFFER_LIGHTING,
  SHADER_FXAA,
  SHADER_WIREFRAME,
  SHADER_NUM_SHADERS
};

struct ShaderSource {
  std::string vertex_source;
  std::string fragment_source;
  std::string geometry_source;
};

ShaderSource parse_shader(const std::string &vertex_shader, const std::string &fragment_shader, const std::string &geometry_shader);
unsigned int compile_shader(unsigned int type, const std::string &source);
unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader, const std::string &geometry_shader);


template <typename K, typename V>
inline V GetWithDef(const  std::map <K,V> & m, const K & key, const V & defval ) {
   typename std::map<K,V>::const_iterator it = m.find( key );
   if ( it == m.end() ) {
      return defval;
   }
   else {
      return it->second;
   }
}


class Shader {
  private:

  public:
    GLuint programID;
    std::map<std::string, bool>  mapped_uniforms;

    std::map <std::string, GLint> uniforms;

    Shader() {};

    void mapUniformLocs(void);
    void set(GLuint shader_id) { this->programID = shader_id; };
    GLuint get(void) { return this->programID; };

    void setVec3(std::string uniform_name, glm::vec3 vec);
    void setMat4(std::string uniform_name, glm::mat4 mat);
    void setInt(std::string uniform_name, GLuint value);
    void setFloat(std::string uniform_name, float value);
    void setFloatVector(std::string uniform_name, int size, float *ptr);
};

