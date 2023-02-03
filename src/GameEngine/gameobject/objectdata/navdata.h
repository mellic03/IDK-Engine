#pragma once

#include "../../computemesh/computemesh.h"
#include "../../../GraphicsEngine/GraphicsEngine.h"
#include "../../util.h"
#include <vector>


namespace Navigation {


  enum class NavFlag {
    NONE      = 0b00000000,
    NOROTATE  = 0b00000001
  };


  enum class NavState {

    NONE,
    REST,
    SEEK

  };


  struct NavData {

    private:
      BitFlag<GLuint, NavFlag> _flags;
      glm::vec3 _target;
      NavMesh *_navmesh;


    public:

      float speed = 0.05f;

      NavState state = NavState::NONE;
      std::vector<glm::vec3> path;

      void setPath(glm::vec3 current_pos, glm::vec3 target, std::list<NavMesh> *navmeshes);
      void followPath(glm::vec3 *objectPos);

      BitFlag<GLuint, NavFlag> *flags()  { return &this->_flags; };
  };
};

