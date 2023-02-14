#include "input.h"


KeyLog Engine::_keylog;


void Engine::processInput(SDL_Event *event, Renderer *ren)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  Engine::_keylog.clear();
  Engine::_keylog.log(state);


  while (SDL_PollEvent(event))
  {
    if (!SDL_GetRelativeMouseMode())
    {
      #ifndef COOM_SHIPPING
        ImGui_ImplSDL2_ProcessEvent(event);
      #endif
    }
    if (event->type == SDL_QUIT)
      exit(0);
    Scene::player.mouse_input(ren, event);
  }
  Scene::player.key_input(ren, state);
}


