#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

// #define COOM_SHIPPING
#ifndef COOM_SHIPPING
  #include "ui/ui.h"
  #include "EditorUI/EditorUI.h"
#endif

#include "GraphicsEngine/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#include "scene/scene.h"
#include "scripting/luascripting.h"
#include "audio/audio.h"


int ENTRY(int argc, const char **argv)
{
  #ifdef COOM_DEBUG
    printf("COOM_DEBUG defined\n");
  #else
    printf("COOM_DEBUG NOT defined\n");
  #endif

  #ifdef COOM_SHIPPING
    printf("COOM_SHIPPING defined\n");
  #else
    printf("COOM_SHIPPING NOT defined\n");
  #endif


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
    1000,
    1000,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
  );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(0); // vsync
  SDL_SetRelativeMouseMode(SDL_FALSE);

  if (glewInit() != GLEW_OK)
    return 1;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // GLCALL( glEnable(GL_DEPTH_CLAMP) );

  SDL_Event event;


  // ENGINE SETUP
  //----------------------------------------
  Renderer *ren = &Render::ren;
  ren->init();

  AudioEngine::init();

  Scene::init();
  Scene::scenegraph.loadObjects("assets/index.txt");
  Scene::defaultScene();
  Scene::importScene("assets/scenes/entry.scene");

  luaInit();
  //----------------------------------------



  // IMGUI SETUP
  //----------------------------------------
  #ifndef COOM_SHIPPING
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
  #endif
  //----------------------------------------



  // RENDER LOOP
  //----------------------------------------
  Uint64 start = SDL_GetPerformanceCounter(), end = SDL_GetPerformanceCounter();
  while (1)
  {
    glm::vec3 v = Scene::player.cam->front;
    v = glm::normalize(v);
    AudioEngine::listener_dir = &v;
    AudioEngine::listener_pos = Scene::player.getPos();

    start = end;
    end = SDL_GetPerformanceCounter();
    int x, y, w, h;

    #ifndef COOM_SHIPPING
      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();
      draw_ui(ren, &x, &y, &w, &h);
    #else
      SDL_GetWindowSize(window, &w, &h);
    #endif

    if (ren->viewport_width != w || ren->viewport_height != h)
      ren->resize(w, h);

    ren->perFrameUpdate();

    // Input
    //---------------------------------
    while (SDL_PollEvent(&event))
    {
      if (!SDL_GetRelativeMouseMode())
      {
        #ifndef COOM_SHIPPING
          ImGui_ImplSDL2_ProcessEvent(&event);
        #endif
      }
      if (event.type == SDL_QUIT)
        exit(0);
      Scene::player.mouse_input(ren, &event);
    }
    Scene::player.key_input(ren);
    //---------------------------------

    ///////////////////////////////////////////////////////////////////////////////////////////// Render start

    Scene::perFrameUpdate();



    // Render depthmaps
    //---------------------------------
    Scene::drawDepthmaps();
    //---------------------------------


    // G-Buffer geometry pass
    //---------------------------------
    GLCALL( glViewport(0, 0, w, h) );
    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, ren->gbufferFBO) );
    GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
    Scene::drawBackground();
    Scene::drawGeometry_batched();
    //---------------------------------



    // G-Buffer lighting pass
    //---------------------------------
    GLCALL(glViewport(0, 0, w, h));
    glBindFramebuffer(GL_FRAMEBUFFER, ren->colorFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    ren->useShader(SHADER_GBUFFER_LIGHTING);
    Scene::sendLightsToShader();

    ren->active_shader->setMat4("view", ren->cam.view);
    ren->active_shader->setMat4("projection", ren->cam.projection);

    for (int i=0; i<NUM_SHADOW_CASCADES; i++)
    {
      std::string stringi = std::to_string(i);

      ren->active_shader->setFloat("shadow_cascades[" + stringi + "]", ren->cascaded_rsm.cascade_distances[i]);
      ren->active_shader->setMat4("dir_lightSpaceMatrices[" + stringi + "]", ren->cascaded_rsm.lightSpaceMatrices[i]);

      glActiveTexture(GL_TEXTURE5 + i);
      glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.depthArray[i]);
      ren->active_shader->setInt("dir_depthmaps[" + stringi + "]", 5 + i);
    }



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

    ren->active_shader->setMat4("projection", ren->cam.projection);
    ren->active_shader->setMat4("view", ren->cam.view);
    for (int i=0; i<NUM_SHADOW_CASCADES; i++)
    {
      ren->active_shader->setFloat("shadow_cascades[" + std::to_string(i) + "]", ren->cascaded_rsm.cascade_distances[i]);
      ren->active_shader->setMat4("dir_lightSpaceMatrices[" + std::to_string(i) + "]", ren->cascaded_rsm.lightSpaceMatrices[i]);

      glActiveTexture(GL_TEXTURE5 + i);
      glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.depthArray[i]);
      ren->active_shader->setInt("dir_depthmaps[" + std::to_string(i) + "]", 5 + i);
    }

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.positionArray[2]);
    ren->active_shader->setInt("dir_positionArraytest", 12);

    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.normalArray[2]);
    ren->active_shader->setInt("dir_normalArraytest", 13);




    Scene::sendLightsToShader();
    Scene::sendVolumetricData();
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

    #ifndef COOM_SHIPPING
      glBindFramebuffer(GL_FRAMEBUFFER, ren->screenQuadFBO);
    #else
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #endif

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


    #ifndef COOM_SHIPPING
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #else

    #endif

  
    SDL_GL_SwapWindow(window);
    double dtime_milliseconds = ((end - start)*1000 / (double)SDL_GetPerformanceFrequency() );
    
    #ifndef COOM_SHIPPING
      ren->deltaTime = io.DeltaTime;
    #else
      ren->deltaTime = dtime_milliseconds / 1000;
    #endif

    luaMain(ren, &Scene::player, &Scene::scenegraph.m_object_instances);
  }
  //----------------------------------------


  return 0;
}
