#pragma once

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/imgui/imgui_stdlib.h"

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../scene/scene.h"
#include "../scripting/luainterface.h"

#include "filebrowser.h"


namespace EngineUI {

  extern int selected_objectID;
  extern FileBrowser filebrowser;

  extern char script_buffer[4096];
  extern ImGuiInputFlags script_flags;

  extern std::stringstream string_stream;

  void vec3(std::string name, glm::vec3 *data, float step);

  void sceneHierarchy(Renderer *ren, Scene *scene);
  void properties(Scene *scene);
  void scriptBrowser(void);
  void scriptEditor(void);
  void details(Scene *scene);

};