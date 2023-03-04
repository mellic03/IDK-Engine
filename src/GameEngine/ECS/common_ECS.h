#pragma once

#include "../common.h"
#include "../gameobject/gameobject.h"

namespace ECS {
  
  enum class ComponentType: Uint32 {
    NONE,
    GEOMETRY,
    SPHERE_COLLIDER,
    MESH_COLLIDER,
    NUM_COMPONENTS
  };

  class EntityComponentSystem;

};

