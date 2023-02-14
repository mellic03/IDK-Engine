#pragma once

#include "../../physics.h"
#include "../../common.h"


enum class PhysicsFlag: GLuint {
  NONE        = 0,
  VELOCITY    = 1 << 0,
  GRAVITY     = 1 << 1
};



struct PhysicsData {

  private:

    BitFlag<GLuint, PhysicsFlag> _flags;


  public:

    glm::vec3 collider_radius = glm::vec3(0.0f);
    float collider_height_offset = 0.0f;
    PhysicsState state;

    BitFlag<GLuint, PhysicsFlag> *flags() { return &this->_flags; };

};

