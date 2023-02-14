#include "ECS.h"



void ECS::EntityComponentSystem::processEntities()
{
  for (ECS::Entity &entity: this->_entities)
  {
    for (Uint32 i = 0; i < (Uint32)ECS::ComponentType::NUM_COMPONENTS; i++)
    {

    }
  }
}

