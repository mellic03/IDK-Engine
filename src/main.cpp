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
#include "model.h"


int main(int argc, char **argv)
{
  SDL_Window *window = NULL;
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
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  SDL_Event event;

  Camera cam;
  ShaderSource ssrc = parse_shader("./src/shaders/basic.glsl");
  GLuint shader = create_shader(ssrc.vertex_source, ssrc.fragment_source);

  Model model_1;
  model_1.load("assets/cube/cube.obj", shader);

  Model ground;
  ground.load("assets/cube/ground.obj", shader);

  Model skybox;
  skybox.load("assets/cube/skybox.obj", shader);
  // RENDER LOOP
  //----------------------------------------
  Uint32 start, end;
  while (1)
  {
    start = SDL_GetTicks();
    cam.input(&event);

    glClearColor(0, 0.5, 0.7, 0.8);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    model_1.draw(&cam, shader);
    ground.draw(&cam, shader);
    skybox.draw(&cam, shader);


    SDL_GL_SwapWindow(window);

    end = SDL_GetTicks();

    float dtime = (end - start) / 1000.0f;

  }
  //----------------------------------------


  return 0;
}