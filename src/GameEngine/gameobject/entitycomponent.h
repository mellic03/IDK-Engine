#pragma once

#include <vector>
#include <functional>
#include <filesystem>

#include <stdio.h>


#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_sdl.h"
#include "../../include/imgui/imgui_impl_opengl3.h"


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


  public:
    std::filesystem::path script_path;
    std::string script_name = "LuaScripting/scripts/default";

    EntityComponent(EntityComponentType component_type); 
    void draw(GameObject *object);

};



namespace EntityComponentUI {

  bool newComponent(EntityComponentType *component_type);

};