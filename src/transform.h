#pragma once

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/quaternion.hpp"
#include "include/glm/gtx/quaternion.hpp"

#include <stdio.h>

struct Transform {

  Transform(void) { };

  Transform *parent = nullptr;

  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 last_rotation  = {0.0f, 0.0f, 0.0f};
  glm::quat rotation_q     = glm::quat(glm::radians(this->rotation));

  // Member access
  //-------------------------------------------------------------------------------------------------------------
  glm::vec3 *getPos(void)       { return &this->position; };
  glm::vec3 *getVel(void)       { return &this->velocity; };
  glm::vec3 *getRot(void)       { this->rotation_q = glm::quat(glm::radians(this->rotation)); return &this->rotation; };
  glm::quat *getRotQ(void)      { return &this->rotation_q; };

  glm::vec4 getPos_vec4(void)   { return glm::vec4(this->position.x, this->position.y, this->position.z, 1.0f); };
  glm::vec4 getVel_vec4(void)   { return glm::vec4(this->velocity.x, this->velocity.y, this->velocity.z, 0.0f); };
  glm::vec4 getRot_vec4(void)   { return glm::vec4(this->rotation.x, this->rotation.y, this->rotation.z, 0.0f); };

  glm::vec3 getPos_worldspace(void)   { return this->getModelMatrix_noLocalTransform() * this->getPos_vec4(); };


  glm::mat4 getModelMatrix(void)
  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation_q);

    if (this->parent != nullptr)
      return this->parent->getModelMatrix() * model;
    else
      return model;
  };

  /** Return the model matrix without applying local translation/rotation.
   *  Essentially returns the model matrix of the parent.
  */
  glm::mat4 getModelMatrix_noLocalTransform(void)
  {
    glm::mat4 model = glm::mat4(1.0f);
    if (this->parent != nullptr)
      return this->parent->getModelMatrix() * model;
    else
      return model;
  };
  //-------------------------------------------------------------------------------------------------------------


  glm::vec3 worldToLocal(glm::vec4 vec)
  {
    return glm::inverse(this->getModelMatrix()) * vec;
  };
  
  glm::vec3 worldToLocal(glm::vec3 vec, bool directional)
  {
    return glm::inverse(this->getModelMatrix()) * glm::vec4(vec.x, vec.y, vec.z, (directional) ? 0.0f : 1.0f);
  };

  glm::vec3 worldToLocal_noLocalTransform(glm::vec4 vec)
  {
    return glm::inverse(this->getModelMatrix_noLocalTransform()) * vec;
  };
  
  glm::vec3 worldToLocal_noLocalTransform(glm::vec3 vec, bool directional)
  {
    return glm::inverse(this->getModelMatrix_noLocalTransform()) * glm::vec4(vec.x, vec.y, vec.z, (directional) ? 0.0f : 1.0f);
  };


  glm::vec3 localToWorld(glm::vec4 vec)
  {
    return this->getModelMatrix() * vec;
  };
  glm::vec3 localToWorld(glm::vec3 vec, bool directional)
  {
    return this->getModelMatrix() * glm::vec4(vec.x, vec.y, vec.z, (directional) ? 0.0f : 1.0f);;
  };

  glm::vec3 localToWorld_noLocalTransform(glm::vec4 vec)
  {
    return this->getModelMatrix_noLocalTransform() * vec;
  };
  glm::vec3 localToWorld_noLocalTransform(glm::vec3 vec, bool directional)
  {
    return this->getModelMatrix_noLocalTransform() * glm::vec4(vec.x, vec.y, vec.z, (directional) ? 0.0f : 1.0f);;
  };

};