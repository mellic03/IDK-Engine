#pragma once

#include "common_ECS.h"
#include "component.h"


class ECS::Entity {

  public:
    int id = -1;

    bool components[(unsigned long)ComponentType::NUM_COMPONENTS] = { false };
    unsigned num_components[(unsigned long)ComponentType::NUM_COMPONENTS] = { 0 };
  
    Entity()
    {
      for (int i=0; i<(unsigned long)ComponentType::NUM_COMPONENTS; i++)
      {
        this->components[i] = false;
        this->num_components[i] = 0;
      }
    };

    void addComponent(ComponentType type);
    void removeComponent(ComponentType type, int index);
    bool hasComponent(ComponentType type) { return this->components[(int)type]; };
};


