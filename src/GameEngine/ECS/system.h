#pragma once
#include "component.h"
#include "entity.h"



void ECS::System::geometry(ECS::Entity entity)
{
  if (entity.hasComponent(ComponentType::GEOMETRY) == false)
    return;
  
}