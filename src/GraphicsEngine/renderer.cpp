#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include "renderer.h"

// float REN_fov = 65.0f;
// float REN_deltaTime = 0.0f;
// Camera REN_active_cam;
// GLuint REN_active_shader;

Renderer renderer;

Renderer::Renderer()
{
}

ShaderSource parse_shader(const std::string &filepath)
{
  std::ifstream stream(filepath);
  
  ShaderType mode;

  std::string line;
  std::stringstream ss[2];
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
        mode = SHADER_VERTEX;
      else if (line.find("fragment") != std::string::npos)
        mode = SHADER_FRAGMENT;
    }
    else
    {
      ss[mode] << line << '\n';
    }
  }

  return (ShaderSource){ss[0].str(), ss[1].str()};
}

unsigned int compile_shader(unsigned int type, const std::string &source)
{
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n" << std::endl; 
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}


void Shader::setVec3(const char *uniform_name, glm::vec3 vec)
{
  int uniform_loc = glGetUniformLocation(this->id, uniform_name);
  glUniform3fv(uniform_loc, 1, glm::value_ptr(vec));
}

void Shader::setMat4(const char *uniform_name, glm::mat4 mat)
{
  int uniform_loc = glGetUniformLocation(this->id, uniform_name);
  glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setFloat(const char *uniform_name, float value)
{
  int uniform_loc = glGetUniformLocation(this->id, uniform_name);
  glUniform1f(uniform_loc, value);

}
