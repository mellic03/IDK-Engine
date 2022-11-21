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

#include "GraphicsEngine/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/renderer.h"

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
    renderer.SCR_width,
    renderer.SCR_height,
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

  ShaderSource basic = parse_shader("assets/shaders/basic.glsl");
  renderer.mat_shader = create_shader(basic.vertex_source, basic.fragment_source);
  renderer.shader.set(create_shader(basic.vertex_source, basic.fragment_source));
  renderer.shader.use();

  Player player;

  Model skybox;  skybox.load("assets/model/skybox.obj");
  Model cube;    cube.load("assets/model/cube.obj");
  Model ground;  ground.load("assets/model/ground.obj");
  

  ModelContainer render_container;
  render_container.add(&cube);
  render_container.add(&ground);
  render_container.add(&skybox);

  ModelContainer physics_container;
  physics_container.add(&cube);
  physics_container.add(&ground);




  // IMGUI
  //----------------------------------------
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 330");
  //----------------------------------------





  // RENDER LOOP
  //----------------------------------------
  cube.translate(glm::vec3(2.0f, -5.8f, 0.0f));
  renderer.lightsource.position = {0.0f, -5.8f, 0.0f};

  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();

    SDL_GetWindowSize(window, &renderer.SCR_width, &renderer.SCR_height);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();



    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");

    Model *modelptr = render_container.head;
    while (modelptr != NULL)
    {
      ImGui::Text("name: %s: pos: %f %f %f", modelptr->name, modelptr->pos.x, modelptr->pos.y, modelptr->pos.z);
      modelptr = modelptr->next;
    }

    ImGui::SliderFloat("FOV", &renderer.fov, 45.0f, 110.0f);
    ImGui::ColorEdit3("ambient", (float*)&renderer.lightsource.ambient);
    ImGui::ColorEdit3("diffuse", (float*)&renderer.lightsource.diffuse);
    ImGui::ColorEdit3("specular", (float*)&renderer.lightsource.specular);
    // if (ImGui::Button("Button"))
    //     counter++;
    // ImGui::SameLine();
    // ImGui::Text("counter = %d", counter);

    ImGui::Text("P(x, y, z): %.2f, %.2f, %.2f", player.pos->x, player.pos->y, player.pos->z);
    ImGui::Text("V(x, y, z): %.2f, %.2f, %.2f", player.vel.x, player.vel.y, player.vel.z);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();


    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    while (SDL_PollEvent(&event))
    {
      if (!SDL_GetRelativeMouseMode())
        ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        exit(0);
      player.mouse_input(&event);
    }
    

    physics_container.collide(&player);
    render_container.draw();
    player.key_input();


    glClear(GL_DEPTH_BUFFER_BIT);
    player.draw();

    cube.rot_y(0.2f);
    cube.rot_x(0.4f);


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    renderer.deltaTime = dtime_milliseconds / 1000;
  }
  //----------------------------------------


  return 0;
}
