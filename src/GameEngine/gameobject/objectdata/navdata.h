#pragma once

#include "../../computemesh/computemesh.h"
#include "../../../GraphicsEngine/GraphicsEngine.h"
#include <vector>


namespace Navigation {

  enum class NavState {

    NONE,
    REST,
    SEEK

  };

  struct NavData {

    private:

      NavState _state = NavState::NONE;
      std::vector<glm::vec3> _path;
      glm::vec3 _target;


    public:

      inline NavState *state();
      
      void setPath(glm::vec3 target);
      void followPath(glm::vec3 *objectPos);

  };
};

