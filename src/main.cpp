#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include "GraphicsEngine/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#include "ui/ui.h"
#include "scene/scene.h"

#include "scripting/luascripting.h"
#include "audio/audio.h"


int ENTRY(int argc, const char **argv)
{

  #ifdef COOMDEBUG
    printf("COOMDEBUG defined\n");
  #else
    printf("COOMDEBUG NOT defined\n");
  #endif


  SDL_Window *window = NULL;
  SDL_GLContext gl_context;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "Error creating window\n";
    return 1;
  }

  AudioEngine::init();

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


  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(0); // vsync
  SDL_SetRelativeMouseMode(SDL_FALSE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);


  SDL_Event event;


  // RENDERER SETUP
  //----------------------------------------
  Renderer *ren = &Render::ren;
  ren->init();
  Player player(ren);

  SceneGraph scenegraph;
  scenegraph.loadObjects("assets/index.txt");

  Scene *scene = &World::scene;
  scene->useSceneGraph(&scenegraph);
  scene->usePlayer(&player);
  scene->defaultScene();

  scene->m_scenegraph->importScene("assets/scenes/entry.scene", &player);

  luaInit(scene, ren);


  scene->useRenderer(ren);
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

  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
  ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
  io.Fonts->AddFontFromFileTTF( "src/fontawesome-webfont.ttf", 16.0f, &icons_config, icons_ranges );

  ImGui::StyleColorsLight();
  
  //----------------------------------------


  // RENDER LOOP
  //----------------------------------------

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));  


  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    glm::vec3 v = player.cam->front;
    v = glm::normalize(v);
    AudioEngine::listener_dir = &v;
    AudioEngine::listener_pos = player.getPos();

    start = end;
    end = SDL_GetPerformanceCounter();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    int x, y, w, h;
    draw_ui(ren, scene, &player, &x, &y, &w, &h, "--dev2");

    ren->perFrameUpdate();

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

    scene->perFrameUpdate();



    // Render depthmaps
    //---------------------------------
    GLCALL(glEnable(GL_DEPTH_CLAMP));
    scene->drawDepthmaps();
    GLCALL(glDisable(GL_DEPTH_CLAMP));
    //---------------------------------


    // G-Buffer geometry pass
    //---------------------------------
    GLCALL(glViewport(0, 0, w, h));
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, ren->gbufferFBO));
    GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    scene->drawBackground();
    scene->drawGeometry_batched();
    //---------------------------------



    // G-Buffer lighting pass
    //---------------------------------
    GLCALL(glViewport(0, 0, w, h));
    glBindFramebuffer(GL_FRAMEBUFFER, ren->colorFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    ren->useShader(SHADER_GBUFFER_LIGHTING);
    scene->sendLightsToShader();

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
    scene->sendLightsToShader();
    ren->sendVolumetricData();
    ren->active_shader->setVec3("viewDir", ren->cam.front);
    
    glBindVertexArray(ren->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, ren->lightshaftColorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------



    // Draw to quad
    //---------------------------------
    GLCALL(glViewport(0, 0, w, h));
    glDisable(GL_DEPTH_TEST);

    ren->useShader(SHADER_SCREENQUAD);
    ren->postProcess();
    
    glBindFramebuffer(GL_FRAMEBUFFER, ren->generalFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
   
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, ren->colorBuffer);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, ren->lightshaftColorBuffer);

    ren->active_shader->setInt("screenTexture", 10);
    ren->active_shader->setInt("volumetricLightsTexture", 11);
    ren->active_shader->setFloat("bloom_threshold", ren->bloom.threshold);

    
    glBindVertexArray(ren->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------

    ren->blurTexture(ren->generalColorBuffers[1], ren->billboardFBO);


    // FXAA
    //---------------------------------
    GLCALL(glViewport(0, 0, w, h));

    ren->useShader(SHADER_FXAA);
    glBindFramebuffer(GL_FRAMEBUFFER, ren->screenQuadFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, ren->generalColorBuffers[0]);
    ren->active_shader->setInt("screenTexture", 10);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, ren->billboardColorBuffer);
    ren->active_shader->setInt("bloomTexture", 11);

    ren->active_shader->setFloat("bloomAmount", ren->bloom.strength);

    glBindVertexArray(ren->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //---------------------------------


    glEnable(GL_DEPTH_TEST);
    ///////////////////////////////////////////////////////////////////////////////////////////// Render stop
    glm::vec2 pt1 = glm::vec2(0.0, 0.0);
    glm::vec2 pt2 = glm::vec2(1.0, 1.0);

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(pt1.x, pt1.y);
    glVertex2f(pt2.x, pt2.y);
    glEnd();

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
