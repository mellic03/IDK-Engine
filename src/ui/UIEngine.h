#pragma once

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/imgui/imgui_stdlib.h"

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../scene/scene.h"
#include "../scripting/luainterface.h"

#include "filebrowser.h"
#include "../IconsFontAwesome4.h"


namespace EngineUI {

  extern int selected_objectID;
  extern FileBrowser filebrowser;

  extern char script_buffer[4096];
  extern ImGuiInputFlags script_flags;

  extern std::stringstream string_stream;

  void dragVec3(std::string name, glm::vec3 *data, float min, float max, float speed, const char *format, float default_value);

  void bitFlagCheckBox(const char *label, unsigned char flag, unsigned char *bits);

  void sceneHierarchy(Renderer *ren);
  void properties();
  void scriptBrowser();
  void scriptEditor();
  void details();
  void debug(Renderer *ren);
  void gameobjects();


  std::string getObjectIcon(GameObjectType type);

};