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





  float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
  };

  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));









  // RENDERER SETUP
  //----------------------------------------
  Renderer ren, shadowren;
  Player player(&ren);
  // Model skybox;  skybox.load("assets/model/", "skybox");  skybox.setShader(cam.shaders[SHADER_WORLDSPACE]);
  Model cube;    cube.load("assets/block/", "block");       cube.setShader(&ren.shaders[SHADER_WORLDSPACE]);
  Model ground;  ground.load("assets/ground/", "ground");   ground.setShader(&ren.shaders[SHADER_WORLDSPACE]);
  Model sphere;  sphere.load("assets/sphere/", "sphere");   sphere.setShader(&ren.shaders[SHADER_LIGHTSOURCE]);
  sphere.bindRenderer(&ren);

  ModelContainer render_container;
  render_container.add(&cube);
  render_container.add(&ground);
  // render_container.add(&skybox);
  render_container.bindRenderer(&ren);


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

    int err = glGetError();
    if (err)
    {
      printf("OpenGL Error: %d\n", err);
      exit(1);
    }
  // RENDER LOOP
  //----------------------------------------
  cube.translate(glm::vec3(2.0f, -5.8f, 0.0f));


  glBindFramebuffer(GL_FRAMEBUFFER, ren.FBO);
 
  GLuint textureColorbuffer, textureDepthBuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


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

    while (SDL_PollEvent(&event))
    {
      if (!SDL_GetRelativeMouseMode())
        ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        exit(0);
      player.mouse_input(&ren, &event);
    }
    

    // first pass
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, ren.FBO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)io.DisplaySize.x, (int)io.DisplaySize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)io.DisplaySize.x, (int)io.DisplaySize.y);  


    physics_container.collide(&player);
    render_container.draw(&ren);

    ren.spotlights[0].position = *player.pos;
    ren.spotlights[0].direction = ren.cam.front;

    for (int i=0; i<NUM_DIRLIGHTS; i++)
    {
      sphere.set_pos(ren.dirlights[i].direction);
      sphere.material.diffuse_color = ren.dirlights[i].diffuse;
      sphere.draw(&ren);
    }

    for (int i=0; i<NUM_POINTLIGHTS; i++)
    {
      sphere.set_pos(ren.pointlights[i].position);
      sphere.material.diffuse_color = ren.pointlights[i].diffuse;
      sphere.draw(&ren);
    }

    player.key_input(&ren);


    glClear(GL_DEPTH_BUFFER_BIT);
    player.draw(&ren);


      
    // second pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
      
    ren.shaders[SHADER_SHADOWMAP].use();
    ren.shaders[SHADER_SHADOWMAP].setInt("screenTexture", 0);
    ren.shaders[SHADER_SHADOWMAP].setFloatVector("kernel", 9, ren.image_kernel);
    ren.shaders[SHADER_SHADOWMAP].setFloat("kernelDivisor", ren.kernel_divisor);
    ren.shaders[SHADER_SHADOWMAP].setFloat("kernelOffsetDivisor", ren.kernel_offset_divisor);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE0, 0);
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6); 






    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    ren.deltaTime = dtime_milliseconds / 1000;

  }
  //----------------------------------------


  return 0;
}
