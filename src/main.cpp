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
#include "ui/ui.h"
#include "scene/scene.h"

#include "LuaScripting/luascripting.h"




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
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
  );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // vsync
  SDL_SetRelativeMouseMode(SDL_FALSE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // glEnable(GL_MULTISAMPLE);


  SDL_Event event;


  luaInit();



  // RENDERER SETUP
  //----------------------------------------
  Renderer ren;
  Player player(&ren);

  SceneGraph scenegraph;

  scenegraph.loadObject("assets/misc/empty/");
  scenegraph.newObjectInstance("empty");
  player.m_gameobject = scenegraph.rearObjectPtr();
  player.m_gameobject->setName("Player");
  player.m_gameobject->setInteractivity("player");


  scenegraph.loadObject("assets/environment/building/");
  scenegraph.loadObject("assets/environment/terrain0/");
  scenegraph.loadObject("assets/environment/terrain1/");
  scenegraph.loadObject("assets/npc/muscleskele/");



  Model sphere;  sphere.loadDae("assets/misc/sphere/", "sphere.dae");  


  Scene scene_1;
  import_lighting_config(&ren);
  scene_1.useRenderer(&ren);
  scene_1.usePlayer(&player);
  scene_1.useSceneGraph(&scenegraph);
  scene_1.addLightsourceModel(&sphere);
  //----------------------------------------


  // IMGUI SETUP
  //----------------------------------------
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 330");
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //----------------------------------------




  // RENDER LOOP
  //----------------------------------------

  int err = glGetError();
  if (err)
  {
    printf("OpenGL Error: %d\n", err);
    exit(1);
  }
  

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);  


  int count = 0;

  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();
    // SDL_GetWindowSize(window, &ren.viewport_width, &ren.viewport_height);
    glClearColor(ren.clearColor.x, ren.clearColor.y, ren.clearColor.z, 1.0f);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    int x, y, w, h;
    draw_dev_ui(&ren, &scene_1, &x, &y, &w, &h);
    ren.update(*player.getPos(), player.cam->front);
    ren.usePerspective();

    // Input
    //---------------------------------
    while (SDL_PollEvent(&event))
    {
      if (!SDL_GetRelativeMouseMode())
        ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        exit(0);
      player.mouse_input(&ren, &event);
    }
    player.key_input(&ren);
    //---------------------------------

    ///////////////////////////////////////////////////////////////////////////////////////////// Render start


    // Render depth map
    // ---------------------------------
    if (count == 1)
    {
      count = 0;
      glViewport(0, 0, ren.SHADOW_WIDTH, ren.SHADOW_HEIGHT);
      glBindFramebuffer(GL_FRAMEBUFFER, ren.depthMapFBO);
      glBindTexture(GL_TEXTURE_CUBE_MAP, ren.depthCubemap);
      glClear(GL_DEPTH_BUFFER_BIT);
          ren.useShader(SHADER_POINTSHADOW);
          ren.setupDepthCubemap({0, 0, 0}, {0, 0, 0});
          glDisable(GL_CULL_FACE);
          scene_1.draw(&event);
          glEnable(GL_CULL_FACE);
      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    else
      count += 1;
    // ---------------------------------



    // Draw scene normally
    // ---------------------------------
    glViewport(0, 0, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, ren.FBO);
    glBindTexture(GL_TEXTURE_2D, ren.colorBuffers[0]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    ren.useShader(SHADER_TERRAIN);
    ren.sendLightsToShader();

    scene_1.draw(&event);

    // glClear(GL_DEPTH_BUFFER_BIT);

    // ren.useShader(SHADER_WEAPON); // switch to viewspace shader
    // ren.sendLightsToShader();
    // player.draw(&ren); // draw weapon
    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------



    
    // Draw to quad
    //---------------------------------
    glBindVertexArray(ren.quadVAO);
    glDisable(GL_DEPTH_TEST);

    ren.useShader(SHADER_SCREENQUAD);
    ren.postProcess();

    glBindFramebuffer(GL_FRAMEBUFFER, ren.screenFBO);
    glBindTexture(GL_TEXTURE_2D, ren.screenColorBuffers[0]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, ren.colorBuffers[0]);
    ren.active_shader->setInt("screenTexture", 10);


    glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // //---------------------------------

    glEnable(GL_DEPTH_TEST);
    ///////////////////////////////////////////////////////////////////////////////////////////// Render stop



    ImGui::Render();


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    ren.deltaTime = dtime_milliseconds / 1000;

    luaMain(&scenegraph.m_object_instances);

  }
  //----------------------------------------


  return 0;
}
