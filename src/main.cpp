#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_sdl.h"
#include "include/imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>

#include "GraphicsEngine/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#include "ui.h"


int ENTRY(int argc, char **argv)
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
    DEFAULT_SCREEN_WIDTH,
    DEFAULT_SCREEN_HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
  );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // vsync
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);  

  SDL_Event event;
  

  // RENDERER SETUP
  //----------------------------------------
  Renderer ren;
  Player player(&ren);
  // Model skybox;  skybox.load("assets/model/", "skybox");  skybox.setShader(cam.shaders[SHADER_WORLDSPACE]);
  Model cube;    cube.load("assets/block/", "block");       cube.setShader(ren.shaders[SHADER_WORLDSPACE]);
  Model ground;  ground.load("assets/ground/", "ground");   ground.setShader(ren.shaders[SHADER_WORLDSPACE]);
  Model sphere;  sphere.load("assets/sphere/", "sphere");   sphere.setShader(ren.shaders[SHADER_LIGHTSOURCE]);

  ModelContainer render_container;
  render_container.add(&cube);
  render_container.add(&ground);
  // render_container.add(&skybox);

  ModelContainer physics_container;
  physics_container.add(&cube);
  physics_container.add(&ground);
  //----------------------------------------



  // IMGUI SETUP
  //----------------------------------------
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 330");
  //----------------------------------------


  // RENDER LOOP
  //----------------------------------------
  cube.translate(glm::vec3(2.0f, -5.8f, 0.0f));

  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();
    SDL_GetWindowSize(window, &ren.SCR_width, &ren.SCR_height);

    draw_dev_ui(&ren);

 
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    while (SDL_PollEvent(&event))
    {
      if (!SDL_GetRelativeMouseMode())
        ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        exit(0);
      player.mouse_input(&ren, &event);
    }
    

    physics_container.collide(&player);
    render_container.draw(&ren);


    for (int i=0; i<8; i++)
    {
      sphere.set_pos(ren.lightsources[i].position);
      sphere.material.diffuse_color = ren.lightsources[i].diffuse;
      sphere.draw(&ren);
    }

    player.key_input(&ren);



    glClear(GL_DEPTH_BUFFER_BIT);
    player.draw(&ren);

    cube.rot_y(0.05f);


    int err = glGetError();
    if (err)
    {
      printf("OpenGL Error: %d\n", err);
      exit(1);
    }


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    ren.deltaTime = dtime_milliseconds / 1000;

  }
  //----------------------------------------


  return 0;
}
