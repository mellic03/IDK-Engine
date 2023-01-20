#include "../../../GameEngine/GameEngine.h"


namespace EntityComponentUI {

  bool newComponent(EntityComponentType *component_type);
  
  void drawComponent_transform(GameObject *object, EntityComponent *entity_component);
  void drawComponent_script(GameObject *object, EntityComponent *entity_component);
  void drawComponent(GameObject *object, EntityComponent *entity_component);

};