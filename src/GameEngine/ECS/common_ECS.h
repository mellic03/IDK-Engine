#pragma once

#include "../common.h"


namespace ECS {
  
  enum class ComponentType: Uint32 {
    NONE,
    GEOMETRY,
    SPHERE_COLLIDER,
    MESH_COLLIDER,
    NUM_COMPONENTS
  };

  class EntityComponentSystem;
  class Entity;


  namespace System {
    void geometry(Entity entity);
    void sphereCollider(Entity entity);
  };

};

