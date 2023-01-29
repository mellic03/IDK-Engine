#include "transformutil.h"


glm::mat4 TransformUtil::lerpMatrix(glm::mat4 A, glm::mat4 B, float alpha)
{
  glm::vec3 pos = glm::mix(glm::vec3(A[3]), glm::vec3(B[3]), alpha);
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);

  glm::quat quat = glm::slerp(glm::quat_cast(A), glm::quat_cast(B), alpha);
  glm::mat4 rotation = glm::mat4_cast(quat);

  return translation * rotation;
}

