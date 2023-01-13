
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

  ShaderSource src;

  src.vertex_source = ss[0].str();
  src.fragment_source = ss[1].str();

  if (geometry_shader == "NULL")
    src.geometry_source = "NULL";
  
  else
    src.geometry_source = ss[2].str();

  return src;
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


void Shader::mapUniformLocs(void)
{
  int count;

  GLint size; // size of the variable
  GLenum type; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei bufSize = 64; // maximum name length
  GLchar name[bufSize]; // variable name in GLSL
  GLsizei length; // name length


  glGetProgramiv(this->id, GL_ACTIVE_UNIFORMS, &count);

  for (int i=0; i<count; i++)
  {
    glGetActiveUniform(this->id, (GLuint)i, bufSize, &length, &size, &type, name);

    this->uniforms[std::string(name)] = glGetUniformLocation(this->id, name);
    this->mapped_uniforms[std::string(name)] = true;

    printf("Uniform #%d Type: %u Name: %s\n", i, type, std::string(name).c_str());
  }
}


void Shader::setVec3(std::string uniform_name, glm::vec3 vec)
{
    this->uniforms[uniform_name] = glGetUniformLocation(this->id, uniform_name.c_str());
  glUniform3fv(this->uniforms[uniform_name], 1, glm::value_ptr(vec));
}

void Shader::setMat4(std::string uniform_name, glm::mat4 mat)
{
    this->uniforms[uniform_name] = glGetUniformLocation(this->id, uniform_name.c_str());
  glUniformMatrix4fv(this->uniforms[uniform_name], 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setInt(std::string uniform_name, GLuint value)
{
    this->uniforms[uniform_name] = glGetUniformLocation(this->id, uniform_name.c_str());
  glUniform1i(this->uniforms[uniform_name], value);
}

void Shader::setFloat(std::string uniform_name, float value)
{
    this->uniforms[uniform_name] = glGetUniformLocation(this->id, uniform_name.c_str());
  glUniform1f(this->uniforms[uniform_name], value);
}

void Shader::setFloatVector(std::string uniform_name, int size, float *ptr)
{
    this->uniforms[uniform_name] = glGetUniformLocation(this->id, uniform_name.c_str());
  glUniform1fv(this->uniforms[uniform_name], size, ptr);
}
