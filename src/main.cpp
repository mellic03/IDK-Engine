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

#include "scripting/luascripting.h"




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
    "Coom Engine",
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  // glEnable(GL_MULTISAMPLE);

  SDL_Event event;


  // RENDERER SETUP
  //----------------------------------------
  Renderer *ren = &Render::ren;
  ren->init();
  Player player(ren);

  SceneGraph scenegraph;

  scenegraph.loadObject("assets/misc/blendingtest/");
  scenegraph.loadObject("assets/misc/empty/");
  scenegraph.loadObject("assets/misc/pointlight/");
  scenegraph.loadObject("assets/misc/spotlight/");
  scenegraph.loadObject("assets/misc/pointlightcontainer/");
  scenegraph.loadObject("assets/misc/spotlightcontainer/");
  scenegraph.loadObject("assets/misc/player/");
  scenegraph.loadObject("assets/environment/skybox/");
  scenegraph.loadObject("assets/environment/building/");
  scenegraph.loadObject("assets/environment/building2/");
  scenegraph.loadObject("assets/environment/terrain0/");
  scenegraph.loadObject("assets/environment/terrain1/");
  scenegraph.loadObject("assets/npc/muscleskele/");
  scenegraph.loadObject("assets/npc/fren/");
  scenegraph.loadObject("assets/props/table/");

  Scene *scene_1 = &World::scene;
  scene_1->useSceneGraph(&scenegraph);
  scene_1->usePlayer(&player);
  scene_1->defaultScene();

  scene_1->m_scenegraph->importScene("assets/scenes/entry.scene", &player);

  luaInit(scene_1, &scenegraph);


  import_lighting_config(ren);
  scene_1->useRenderer(ren);
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
  io.Fonts->AddFontFromFileTTF("./assets/fonts/OpenSans-VariableFont_wdth,wght.ttf", 18.0f);
  ImGui::StyleColorsLight();
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
    // SDL_GetWindowSize(window, ren->viewport_width, ren->viewport_height);
    glClearColor(ren->clearColor.x, ren->clearColor.y, ren->clearColor.z, 1.0f);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    int x, y, w, h;
    draw_dev_ui(ren, scene_1, &player, &x, &y, &w, &h);
    ren->usePerspective();

    // Input
    //---------------------------------
    while (SDL_PollEvent(&event))
    {
      if (!SDL_GetRelativeMouseMode())
        ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        exit(0);
      player.mouse_input(ren, &event);
    }
    player.key_input(ren);
    //---------------------------------

    ///////////////////////////////////////////////////////////////////////////////////////////// Render start


    // Render depth map
    // ---------------------------------
    glViewport(0, 0, ren->SHADOW_WIDTH, ren->SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->depthMapFBO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ren->depthCubemap);
    glClear(GL_DEPTH_BUFFER_BIT);
        ren->useShader(SHADER_POINTSHADOW);
        ren->setupDepthCubemap();
        glDisable(GL_CULL_FACE);
        scene_1->drawGeometry(&event);
        glEnable(GL_CULL_FACE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // ---------------------------------


 
    // Draw scene normally
    // ---------------------------------
    glViewport(0, 0, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->colorFBO);
    glBindTexture(GL_TEXTURE_2D, ren->colorBuffers[0]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ren->useShader(SHADER_TERRAIN);
    scene_1->sendLightsToShader();
    scene_1->drawGeometry(&event);

    ren->useShader(SHADER_LIGHTSOURCE);
    scene_1->drawLightsources(&event);

    //---------------------------------


    // Draw light shafts
    //---------------------------------
    glViewport(0, 0, w/2, h/2);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->lightshaftFBO);
    glBindTexture(GL_TEXTURE_2D, ren->lightshaftColorBuffers[0]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ren->useShader(SHADER_VOLUMETRIC_LIGHT);
    scene_1->sendLightsToShader();
    scene_1->drawVolumetricLights();
    //---------------------------------

    // glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    


    // Draw to quad
    //---------------------------------
    glViewport(0, 0, w, h);
    glBindVertexArray(ren->quadVAO);
    glDisable(GL_DEPTH_TEST);

    ren->useShader(SHADER_SCREENQUAD);
    ren->postProcess();

    glBindFramebuffer(GL_FRAMEBUFFER, ren->screenQuadFBO);
    glBindTexture(GL_TEXTURE_2D, ren->screenQuadColorBuffers[0]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, ren->colorBuffers[0]);
    ren->active_shader->setInt("screenTexture", 10);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, ren->lightshaftColorBuffers[0]);
    ren->active_shader->setInt("volumetricLightsTexture", 11);


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
    ren->deltaTime = dtime_milliseconds / 1000;

    luaMain(ren, &player, &scenegraph.m_object_instances);
  }
  //----------------------------------------


  return 0;
}
