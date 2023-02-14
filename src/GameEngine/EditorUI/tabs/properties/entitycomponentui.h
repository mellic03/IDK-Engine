#pragma once

#include "../../common_EditorUI.h"


namespace EntityComponentUI {

  void newComponent(GameObject *object);
  
  void drawComponent_transform(GameObject *object, EntityComponent *entity_component);
  void drawComponent_script(GameObject *object, EntityComponent *entity_component);
  void drawComponent(GameObject *object, EntityComponents *components, EntityComponentType component_type);

};