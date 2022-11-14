#include "renderer.h"


void engine_input(SDL_Event *event)
{
  while (SDL_PollEvent(event))
  {
    if (event->type == SDL_KEYDOWN)
    {
      switch (event->key.keysym.sym)
      {
        case SDLK_ESCAPE:
          exit(0);
          break;
      }
    }
    
    else if (event->type == SDL_QUIT)
      exit(0);
  }
}