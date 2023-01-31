#pragma once

#include "../../physics.h"




struct PhysicsData {

  private:
    PhysicsState _state;

  public:
    inline PhysicsState *state() { return &this->_state; };

};

