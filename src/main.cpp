#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

#include "GraphicsEngine/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"


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
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);  

  SDL_Event event;

  ShaderSource ssrc = parse_shader("assets/shaders/basic.glsl");
  GLuint shader = create_shader(ssrc.vertex_source, ssrc.fragment_source);

  Player player(shader);

  Model skybox;  skybox.load("assets/cube/skybox.obj", shader);
  Model cube;    cube.load("assets/cube/cube.obj", shader);
  Model ground;  ground.load("assets/cube/ground.obj", shader);
  
  ModelContainer render_container;
  render_container.add(&cube);
  render_container.add(&ground);
  render_container.add(&skybox);

  ModelContainer physics_container;
  physics_container.add(&cube);
  physics_container.add(&ground);

  // RENDER LOOP
  //----------------------------------------
  Uint64 start = SDL_GetPerformanceCounter(), end = 0;
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    physics_container.collide(&player);


    render_container.draw(&player.cam, shader);


    glClear(GL_DEPTH_BUFFER_BIT);
    player.input(&event, shader);

    cube.translate(glm::vec3(0.0f, -0.003f, 0.0f));
    cube.rot_y(0.1f);

    SDL_GL_SwapWindow(window);

    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    double dtime_seconds = dtime_milliseconds / 1000;
    // printf("FPS: %lf\n", 1.0f / dtime_seconds);
  }
  //----------------------------------------


  return 0;
}