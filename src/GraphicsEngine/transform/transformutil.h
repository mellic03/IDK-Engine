#pragma once

#include "transform.h"


namespace TransformUtil {
  
  glm::mat4 lerpMatrix(glm::mat4 A, glm::mat4 B, float alpha);

  glm::mat4 rotationFromPositions(glm::vec3 start, glm::vec3 end);

};
