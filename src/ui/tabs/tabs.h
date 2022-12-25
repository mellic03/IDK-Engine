#pragma once

#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_sdl.h"
#include "../../include/imgui/imgui_impl_opengl3.h"
#include "../../include/imgui/imgui_stdlib.h"

#include "../../GameEngine/GameEngine.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../scene/scene.h"


namespace EngineUI {

  void sceneHierarchy(Renderer *ren, Scene *scene);
  void properties(Scene *scene);
  void scriptEditor();

};
