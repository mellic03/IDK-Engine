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

#include "renderer.h"
#include "model.h"

#define SCR_WDTH 1000
#define SCR_HGHT 1000

static ShaderSource parse_shader(const std::string &filepath)
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

static unsigned int compile_shader(unsigned int type, const std::string &source)
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

static unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader)
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

int main(int argc, char **argv)
{
  SDL_Window *window = NULL;
  SDL_Surface *screen_surface = NULL;
  SDL_GLContext gl_context;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "Error creating window\n";
    return 1;
  }

  window = SDL_CreateWindow(
    "Test OpenGL",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCR_WDTH,
    SCR_HGHT,
    SDL_WINDOW_OPENGL
  );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);

  if (glewInit() != GLEW_OK)
    return 0;


  screen_surface = SDL_GetWindowSurface(window);
  SDL_UpdateWindowSurface(window);

  SDL_Event event;
  bool running = true;

  Model model_1;

  ShaderSource ssrc = parse_shader("./src/shaders/basic.glsl");

  unsigned int shader = create_shader(ssrc.vertex_source, ssrc.fragment_source);
  glUseProgram(shader);

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            running = false;
            break;
        }
      }
      
      else if (event.type == SDL_QUIT)
        running = 0;
    }

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // RENDER LOOP
    //----------------------------------------

    model_1.draw();

    //----------------------------------------
    SDL_GL_SwapWindow(window);
  }

  glDeleteProgram(shader);

  return 0;
}