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
  // glEnable(GL_BLEND);
  glDisable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  // glEnable(GL_MULTISAMPLE);

  SDL_Event event;


  // RENDERER SETUP
  //----------------------------------------
  Renderer *ren = &Render::ren;
  ren->init();
  Player player(ren);

  SceneGraph scenegraph;

  scenegraph.loadObject("assets/misc/thang/");
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
  scenegraph.loadObject("assets/environment/building3/");
  scenegraph.loadObject("assets/environment/terrain0/");
  scenegraph.loadObject("assets/environment/terrain1/");
  scenegraph.loadObject("assets/environment/terrain2/");
  scenegraph.loadObject("assets/npc/muscleskele/");
  scenegraph.loadObject("assets/npc/fren/");
  scenegraph.loadObject("assets/props/table/");

  Scene *scene_1 = &World::scene;
  scene_1->useSceneGraph(&scenegraph);
  scene_1->usePlayer(&player);
  scene_1->defaultScene();

  scene_1->m_scenegraph->importScene("assets/scenes/entry.scene", &player);

  luaInit(scene_1, &scenegraph);


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


  Shader gbuffer_geometrypass = ren->createShader("gbuffer_geometry");
  Shader gbuffer_lightingpass = ren->createShader("gbuffer_lighting");


  int count = 0;
  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();
    // SDL_GetWindowSize(window, ren->viewport_width, ren->viewport_height);
    // glClearColor(ren->clearColor.x, ren->clearColor.y, ren->clearColor.z, 1.0f);

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



    // Render depthmaps
    //---------------------------------
    scene_1->drawDepthmaps();
    //---------------------------------



    // Draw scene normally
    //---------------------------------
    glViewport(0, 0, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->colorFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ren->useShader(SHADER_TERRAIN);
    scene_1->sendLightsToShader();
    // scene_1->drawGeometry();

    ren->useShader(SHADER_LIGHTSOURCE);
    scene_1->drawLightsources(&event);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------



    // G-Buffer geometry pass
    //---------------------------------
    glViewport(0, 0, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->gbufferFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    ren->useShader(SHADER_GBUFFER_GEOMETRY);
    scene_1->drawGeometry();

    ren->useShader(SHADER_LIGHTSOURCE);
    scene_1->drawLightsources(&event);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------


    // G-Buffer lighting pass
    //---------------------------------
    glViewport(0, 0, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->colorFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    ren->useShader(SHADER_GBUFFER_LIGHTING);
    scene_1->sendLightsToShader();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ren->gbuffer_position);
    ren->active_shader->setInt("gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ren->gbuffer_normal);
    ren->active_shader->setInt("gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ren->gbuffer_albedospec);
    ren->active_shader->setInt("gAlbedoSpec", 2);


    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ren->gbuffer_emission);
    ren->active_shader->setInt("gEmission", 3);


    glBindVertexArray(ren->quadVAO);

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
  
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------



    // Draw light shafts
    //---------------------------------
    glViewport(0, 0, w/ren->volumetrics.resolution_divisor, h/ren->volumetrics.resolution_divisor);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->lightshaftFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ren->useShader(SHADER_VOLUMETRIC_LIGHT);
    scene_1->sendLightsToShader();
    ren->sendVolumetricData();
    
    glBindVertexArray(ren->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------



    // Blur bloom and volumetric light buffers
    //---------------------------------
    glViewport(0, 0, w, h);
    ren->blurTexture( ren->lightshaftFBO, ren->lightshaftColorBuffer,
                      ren->volumetrics.num_blur_passes, ren->volumetrics.texel_size,
                      ren->volumetrics.x_strength, ren->volumetrics.y_strength
                    );
    //---------------------------------
  


    // Draw to quad
    //---------------------------------
    glViewport(0, 0, w, h);
    glBindVertexArray(ren->quadVAO);
    glDisable(GL_DEPTH_TEST);

    ren->useShader(SHADER_SCREENQUAD);
    ren->postProcess();

    glBindFramebuffer(GL_FRAMEBUFFER, ren->screenQuadFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
   
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, ren->colorBuffers[0]);
    ren->active_shader->setInt("screenTexture", 10);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, ren->pingPongColorBuffers[0]);
    ren->active_shader->setInt("volumetricLightsTexture", 11);

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, ren->colorBuffers[1]);
    ren->active_shader->setInt("bloomTexture", 12);


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
    ren->deltaTime = io.DeltaTime;
    
    luaMain(ren, &player, &scenegraph.m_object_instances);
  }
  //----------------------------------------


  return 0;
}
