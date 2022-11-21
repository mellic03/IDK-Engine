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

  Player player;

  Model skybox;  skybox.load("assets/cube/skybox.obj");
  Model cube;    cube.load("assets/cube/cube.obj");
  Model ground;  ground.load("assets/cube/ground.obj");
  

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
  bool show_demo_window = true;
  glm::vec3 color;
  cube.translate(glm::vec3(0.0f, -0.6f, 0.0f));
  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    start = end;
    end = SDL_GetPerformanceCounter();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();



    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    Model *modelptr = render_container.head;
    while (modelptr != NULL)
    {
      ImGui::Text("name: %s", modelptr->name);
      modelptr = modelptr->next;
    }

    ImGui::SliderFloat("float", &renderer.fov, 45.0f, 110.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("ambient", (float*)&renderer.lightsource.ambient); // Edit 3 floats representing a color
    ImGui::ColorEdit3("diffuse", (float*)&renderer.lightsource.diffuse); // Edit 3 floats representing a color
    ImGui::ColorEdit3("specular", (float*)&renderer.lightsource.specular); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("P(x, y, z): %.2f, %.2f, %.2f", player.pos->x, player.pos->y, player.pos->z);
    ImGui::Text("V(x, y, z): %.2f, %.2f, %.2f", player.vel.x, player.vel.y, player.vel.z);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();


    ImGui::Render();


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


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    renderer.deltaTime = dtime_milliseconds / 1000;
  }
  //----------------------------------------


  return 0;
}