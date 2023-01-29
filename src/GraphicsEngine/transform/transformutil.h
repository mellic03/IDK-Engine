#pragma once

#include "transform.h"


namespace TransformUtil {
  
  glm::mat4 lerpMatrix(glm::mat4 A, glm::mat4 B, float alpha);

};
