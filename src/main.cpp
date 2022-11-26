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
#include "scene.h"



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
  Renderer ren, shadowren;
  Player player(&ren);



  // Model skybox;  skybox.load("assets/model/", "skybox");
  Model cube;    cube.load("assets/crate/", "crate");
  Model cube2;   cube2.load("assets/crate/", "crate");
  Model ground;  ground.load("assets/ground/", "ground");
  Model sphere;  sphere.load("assets/sphere/", "sphere");
  sphere.bindRenderer(&ren);



  ModelContainer render_container;
  render_container.add(&cube);
  render_container.add(&cube2);
  render_container.add(&ground);
  // render_container.add(&skybox);
  render_container.bindRenderer(&ren);

  ModelContainer physics_container;
  physics_container.add(&cube);
  physics_container.add(&ground);




  Scene scene_1;
  scene_1.addRenderContainer(&render_container);
  scene_1.addPhysicsContainer(&physics_container);
  scene_1.bindRenderer(&ren);
  scene_1.bindPlayer(&player);
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
  //----------------------------------------


  // RENDER LOOP
  //----------------------------------------
  // cube.translate(glm::vec3(2.0f, -5.8f, 0.0f));

  cube2.translate({0.2, 1.0, 0.0});

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
    draw_dev_ui(&ren);


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
    glViewport(0, 0, x, y); glEnable(GL_DEPTH_TEST);
    

    // Render depth map
    // ---------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ren.depthMapFBO);
    glBindTexture(GL_TEXTURE_2D, ren.depthMap);
    glViewport(0, 0, x, y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, x, y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glClear(GL_DEPTH_BUFFER_BIT);
        ren.useOrthographic(player.pos->x, player.pos->y, player.pos->z);
        glCullFace(GL_FRONT);
        scene_1.draw(&event);
        glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    // ---------------------------------



    // Draw scene normally
    // ---------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ren.FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, ren.rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y); 

    glBindTexture(GL_TEXTURE_2D, ren.colorBuffers[0]);
    glViewport(0, 0, x, y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glClear(GL_COLOR_BUFFER_BIT);


    ren.usePerspective();
    ren.useShader(SHADER_WORLDSPACE);

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, ren.depthMap);
    ren.active_shader.setInt("shadowMap", 12);
    ren.active_shader.setMat4("lightSpaceMatrix", ren.lightSpaceMatrix);
    scene_1.draw(&event);

    glClear(GL_DEPTH_BUFFER_BIT); // clear depth buffer for weapon
    glViewport(0, 0, x, y);
    ren.useShader(SHADER_VIEWSPACE); // switch to viewspace shader
    player.draw(&ren); // draw weapon
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------






    // Draw to quad
    //---------------------------------

    glBindVertexArray(ren.quadVAO);

    // glActiveTexture(GL_TEXTURE5);
    // glBindTexture(GL_TEXTURE_2D, ren.depthMap);
    // ren.useShader(SHADER_RENDERQUAD);
    // ren.active_shader.setInt("depthMap", 5);
    glViewport(0, 0, x, y);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ren.colorBuffers[0]);
    ren.useShader(SHADER_FIN);
    ren.postProcess();
    ren.active_shader.setInt("screenTexture", 0);
    ren.active_shader.setInt("bloomBlur", 0);



    glDrawArrays(GL_TRIANGLES, 0, 6);


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
