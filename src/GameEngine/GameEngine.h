#pragma once

#include "player/player.h"
#include "physics.h"
#include "gameobject/gameobject.h"
#include "scenegraph/scenegraph.h"
#include "scene/scene.h"
#include "computemesh/computemesh.h"
#include "scripting/luascripting.h"
#include "audio/AudioEngine.h"
#include "input/input.h"

#ifndef IDK_SHIPPING
  #include "EditorUI/EditorUI.h"
#endif



namespace Engine {

  void init(Renderer *ren);

  void newFrame(Renderer *ren, int *x, int *y, int *w, int *h);
  void endFrame(SDL_Window *window, Renderer *ren, Uint64 dTime_start, Uint64 dTime_end);

  void geometryPass(Renderer *ren);
  void lightingPass(Renderer *ren);
  void volumetricLightPass(Renderer *ren);
  void combinePass(Renderer *ren);
  void fxaa(Renderer *ren);

  void renderScene(Renderer *ren);

};
