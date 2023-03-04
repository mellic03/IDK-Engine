#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

// #define IDK_SHIPPING
// #define IDK_DEBUG


#include "GraphicsEngine/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#include "Game/Game.h"


int ENTRY(int argc, const char **argv)
{
  SDL_Window *window = NULL;
  SDL_GLContext gl_context;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "Error creating window\n";
    return 1;
  }

  window = SDL_CreateWindow(
    "IDK Engine",
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

  SDL_Event event;


  // Engine setup
  //----------------------------------------
  Renderer *ren = &Render::ren;
  Engine::init(ren);
  //----------------------------------------


  // EditorUI setup
  //----------------------------------------
  #ifndef IDK_SHIPPING
    EditorUI::init(window, gl_context);
  #endif
  //----------------------------------------


  // RENDER LOOP
  //--------------------------------------------------------------------------------
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
    Engine::newFrame(ren, &x, &y, &w, &h);


    ren->perFrameUpdate(w, h);

    Engine::processInput(&event, ren);
    Engine::renderScene(ren);


    Engine::endFrame(window, ren, start, end);
    luaMain(ren, &Engine::_keylog, &Scene::scenegraph.m_object_instances);
  }
  //--------------------------------------------------------------------------------


  return 0;
}
