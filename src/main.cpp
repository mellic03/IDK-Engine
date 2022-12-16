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
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // glEnable(GL_MULTISAMPLE);


  SDL_Event event;



  // RENDERER SETUP
  //----------------------------------------
  Renderer ren;
  Player player(&ren);
  player.getPos()->z = 6.0f;
  import_lighting_config(&ren);

  SceneGraph objhandler;

  Mesh sphere;  sphere.load("assets/misc/sphere/", "sphere");
  
  Model tree;               tree.load("assets/environment/tree/");
  GameObject treeobj;       treeobj.useModel(&tree);

  Model building;           building.load("assets/environment/building/");
  GameObject buildingobj;   buildingobj.useModel(&building);

  Model terrain;            terrain.load("assets/environment/terrain1/");
  GameObject terrainobj;    terrainobj.useModel(&terrain);

  Model terrain2;            terrain2.load("assets/environment/ground/");
  GameObject terrain2obj;    terrain2obj.useModel(&terrain2);


  Model fren;               fren.load("assets/npc/fren/");
  GameObject frenobj;       frenobj.useModel(&fren);

  Model boye;               boye.load("assets/environment/boye/");
  GameObject boyeobj;       boyeobj.useModel(&boye);

  Model empty;              empty.load("assets/misc/empty/");
  GameObject emptyobj;      emptyobj.useModel(&empty);


  NavMesh nav1;
  nav1.load("assets/environment/terrain1/nav.obj");


  objhandler.addObject(&treeobj);
  objhandler.addObject(&buildingobj);
  objhandler.addObject(&terrainobj);
  objhandler.addObject(&terrain2obj);
  objhandler.addObject(&frenobj);
  objhandler.addObject(&boyeobj);
  objhandler.addObject(&emptyobj);

  objhandler.newObjectInstance("empty");
  player.setObjectPtr(objhandler.frontObjectPtr());
  player.objectPtr()->setGivenName("Player");

  objhandler.addObject(player.objectPtr());


  Scene scene_1;
  scene_1.useRenderer(&ren);
  scene_1.usePlayer(&player);
  scene_1.addLightsourceModel(&sphere);
  scene_1.addObjectHandler(&objhandler);
  scene_1.navmesh = nav1;

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

  int err = glGetError();
  if (err)
  {
    printf("OpenGL Error: %d\n", err);
    exit(1);
  }
  

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);  


  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();
    SDL_GetWindowSize(window, &ren.SCR_width, &ren.SCR_height);
    glClearColor(ren.clearColor.x, ren.clearColor.y, ren.clearColor.z, 1.0f);
    draw_dev_ui(&ren, &scene_1);

    ren.update(player.pos_worldspace, player.cam->front);

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
    int x = (int)io.DisplaySize.x, y = (int)io.DisplaySize.y;
    ren.usePerspective();
    glEnable(GL_DEPTH_TEST);


    // Render depth map
    // ---------------------------------
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
    // ---------------------------------



    // Draw scene normally
    // ---------------------------------

    glViewport(0, 0, x, y);
    glBindFramebuffer(GL_FRAMEBUFFER, ren.FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ren.depthCubemap);


    ren.useShader(SHADER_WORLDSPACE);
    ren.sendLightsToShader();
    scene_1.draw(&event);

    glClear(GL_DEPTH_BUFFER_BIT);

    ren.useShader(SHADER_WEAPON); // switch to viewspace shader
    ren.sendLightsToShader();
    player.draw(&ren); // draw weapon
    

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------



    
    // Draw to quad
    //---------------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(ren.quadVAO);


    ren.useShader(SHADER_SCREENQUAD);
    ren.postProcess();
    
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, ren.colorBuffers[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    ren.active_shader->setInt("screenTexture", 10);


    glDrawArrays(GL_TRIANGLES, 0, 6);
  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);

    //---------------------------------


    ///////////////////////////////////////////////////////////////////////////////////////////// Render stop





    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    ren.deltaTime = dtime_milliseconds / 1000;

  }
  //----------------------------------------


  return 0;
}
