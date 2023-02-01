#pragma once

#include "../include/imgui/imgui_stl.h"
#include "../include/imgui/imgui_impl_sdl.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/imgui/imgui_stdlib.h"

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../scene/scene.h"
#include "../scripting/luainterface.h"

#include "filebrowser.h"
#include "../IconsFontAwesome4.h"


#define ICON_COMPONENT_TRANSFORM (ICON_FA_ARROWS)
#define ICON_COMPONENT_TERRAIN   (ICON_FA_TREE)
#define ICON_COMPONENT_SCRIPT    (ICON_FA_FILE_O)
#define ICON_COMPONENT_LOD       (ICON_FA_EYE)
#define ICON_COMPONENT_SPHERE    (ICON_FA_ARROW_CIRCLE_UP)


namespace EngineUI {

  namespace IconLabels {
    extern std::string TRANSFORM_LABEL;
    extern std::string SCRIPT_LABEL;
    extern std::string LOD_LABEL;
    extern std::string SPHERE_LABEL;
    extern std::string CAPSULE_LABEL;
    extern std::string MESH_LABEL;
    extern std::string PHYSICS_LABEL;
  };


  extern int selected_objectID;
  extern GameObject *selected_gameobject_template;

  extern FileBrowser filebrowser;

  extern char script_buffer[4096];
  extern ImGuiInputFlags script_flags;
  extern std::stringstream string_stream;


  void dragVec3(std::string name, glm::vec3 *data, float min, float max, float speed, const char *format, float default_value);

  void bitFlagCheckbox(const char *label, unsigned char flag, unsigned char *bits);

  template <typename T>
  void bitFlagCheckbox(const char *label, T flag, T *bits)
  {
    GLuint flag_casted = (GLuint)flag;
    GLuint bits_casted = (GLuint)(*bits);

    bool active = flag_casted & bits_casted;

    ImGui::Checkbox(label, &active);

    if (active)
      bits_casted = (flag_casted | bits_casted);
    else
      bits_casted = (~flag_casted & bits_casted);

    *bits = (T)bits_casted;
  }


  void sceneHierarchy(Renderer *ren);
  void instanceProperties();
  void scriptBrowser();
  void scriptEditor();
  void details();
  void debug(Renderer *ren);


  void gameobjects();
  void globalProperties();

  std::string getObjectIcon(GameObjectType type);
  std::string getComponentIcon(EntityComponentType type);

};