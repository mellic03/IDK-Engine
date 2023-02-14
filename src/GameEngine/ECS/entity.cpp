#include "entity.h"




void ECS::Entity::addComponent(ECS::ComponentType type)
{
  this->components[(int)type] = true;
  this->num_components[(int)type] += 1;
}


void ECS::Entity::removeComponent(ECS::ComponentType type, int index)
{
  this->components[(int)type] = false;
  this->num_components[(int)type] -= 1;
}

