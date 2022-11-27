
#include "shader.h"

ShaderSource parse_shader(const std::string &vertex_shader, const std::string &fragment_shader, const std::string &geometry_shader)
{
  std::ifstream vs_stream(vertex_shader);
  std::ifstream fs_stream(fragment_shader);
  std::ifstream gs_stream(geometry_shader);
  std::string line;
  std::stringstream ss[3];

  while (getline(vs_stream, line))
    ss[0] << line << '\n';

  while (getline(fs_stream, line))
    ss[1] << line << '\n';

  if (geometry_shader != "NULL")
    while (getline(gs_stream, line))
      ss[2] << line << '\n';

  return (ShaderSource){ss[0].str(), ss[1].str(), geometry_shader == "NULL" ? "NULL" : ss[2].str()};
}

unsigned int compile_shader(unsigned int type, const std::string &source)
{
  if (source == "NULL") return 0;
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

unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader, const std::string &geometry_shader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
  unsigned int gs = geometry_shader == "NULL" ? 0 : compile_shader(GL_GEOMETRY_SHADER, geometry_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  if (gs != 0)  glAttachShader(program, gs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);
  glDeleteShader(gs);

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


void Shader::setInt(const char *uniform_name, GLuint value)
{
  int uniform_loc = glGetUniformLocation(this->id, uniform_name);
  glUniform1i(uniform_loc, value);
}

void Shader::setFloat(const char *uniform_name, float value)
{
  int uniform_loc = glGetUniformLocation(this->id, uniform_name);
  glUniform1f(uniform_loc, value);
}

void Shader::setFloatVector(const char *uniform_name, int size, float *ptr)
{
  int uniform_loc = glGetUniformLocation(this->id, uniform_name);
  glUniform1fv(uniform_loc, size, ptr);
}