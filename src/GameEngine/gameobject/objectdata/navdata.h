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
      glm::vec3 _target;

      NavMesh *_navmesh;


    public:

      NavState state = NavState::NONE;
      std::vector<glm::vec3> path;

      void setPath(glm::vec3 current_pos, glm::vec3 target, std::list<NavMesh> *navmeshes);
      void followPath(glm::vec3 *objectPos);

  };
};

