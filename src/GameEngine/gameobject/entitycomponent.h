#pragma once

#include <vector>
#include <functional>
#include <filesystem>

#include <stdio.h>


#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_sdl.h"
#include "../../include/imgui/imgui_impl_opengl3.h"

#include "../../GraphicsEngine/GraphicsEngine.h"

#include "gameobject.h"
class GameObject;


enum EntityComponentType {
  COMPONENT_NONE = 0,
  COMPONENT_TRANSFORM,
  COMPONENT_LIGHTSOURCE,
  COMPONENT_SCRIPT
};


class EntityComponent { 

  private:
    EntityComponentType _component_type = COMPONENT_NONE;
    PointLight *_pointlight;
    SpotLight *_spotlight;

    void _draw_pointlight(void);
    void _draw_spotlight(void);
    void _draw_lightsource(void);

    void _draw_transform(GameObject *object);
    void _draw_script(GameObject *object);


  public:
    std::filesystem::path script_path;
    std::string script_name = "LuaScripting/scripts/default";

    EntityComponent(EntityComponentType component_type);
    EntityComponent(EntityComponentType component_type, PointLight *pointlight);
    EntityComponent(EntityComponentType component_type, SpotLight *spotlight);



    void draw(GameObject *object);

};



namespace EntityComponentUI {

  bool newComponent(EntityComponentType *component_type);

};