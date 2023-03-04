#pragma once

#include "common_ECS.h"


namespace ECS {


  struct SphereCollider {
    glm::vec3 relative_position = glm::vec3(0.0f);
    float radius = 0.0f;
  };


  struct MeshCollider {
    
  };


  struct Geometry {
    Model *model = nullptr;
  };


};
