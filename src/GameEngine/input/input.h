#pragma once

#include "../common.h"
#include "../scene/scene.h"
#include "../EditorUI/common_EditorUI.h"

namespace Engine {

  extern KeyLog _keylog;

  void processInput(SDL_Event *event, Renderer *ren);

};


