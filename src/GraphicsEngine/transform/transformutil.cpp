#include "transformutil.h"


glm::mat4 TransformUtil::lerpMatrix(glm::mat4 A, glm::mat4 B, float alpha)
{
  glm::vec3 pos = glm::mix(glm::vec3(A[3]), glm::vec3(B[3]), alpha);
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);

  glm::quat quat = glm::slerp(glm::quat_cast(A), glm::quat_cast(B), alpha);
  glm::mat4 rotation = glm::mat4_cast(quat);

  return translation * rotation;
}


glm::mat4 TransformUtil::rotationFromPositions(glm::vec3 start, glm::vec3 end)
{
  glm::vec3 dir = glm::normalize(end - start);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(dir, up));

  glm::mat4 rotation = glm::mat4(1.0f);
  rotation[0][0] = right.x;
  rotation[1][0] = right.y;
  rotation[2][0] = right.z;
  rotation[0][1] = up.x;
  rotation[1][1] = up.y;
  rotation[2][1] = up.z;
  rotation[0][2] = -dir.x;
  rotation[1][2] = -dir.y;
  rotation[2][2] = -dir.z;

  return rotation;
}


