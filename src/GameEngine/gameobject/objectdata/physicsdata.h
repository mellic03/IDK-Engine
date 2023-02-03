#pragma once

#include "../../physics.h"




struct PhysicsData {

  private:


  public:
  
    glm::vec3 collider_radius = glm::vec3(0.0f);
    float collider_height_offset = 0.0f;
    PhysicsState state;

    

};

