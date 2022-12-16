#pragma once

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

struct Transform {

  int *m_ID = nullptr;

  Transform(void)    { this->m_ID = nullptr; };
  Transform(int *id) { this->m_ID = id; };

  Transform *parent = nullptr;

  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

  // Direct member access
  //-------------------------------------------------------------------------------------------------------------
  glm::vec3 *getPos(void)       { return &this->position; };
  glm::vec3 *getVel(void)       { return &this->velocity; };
  glm::vec3 *getRot(void)       { return &this->rotation; };

  glm::vec4 getPos_vec4(void)   { return glm::vec4(this->position.x, this->position.y, this->position.z, 1.0f); };
  glm::vec4 getVel_vec4(void)   { return glm::vec4(this->velocity.x, this->velocity.y, this->velocity.z, 0.0f); };
  glm::vec4 getRot_vec4(void)   { return glm::vec4(this->rotation.x, this->rotation.y, this->rotation.z, 0.0f); };
  //-------------------------------------------------------------------------------------------------------------



  // Utility member access
  //-------------------------------------------------------------------------------------------------------------
  glm::vec3 getPos_worldspace(void)   { return this->getModelMatrix_noLocalTransform() * this->getPos_vec4(); };

  glm::mat4 getModelMatrix(void)
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);

    model = glm::rotate(model, glm::radians(this->rotation.x), {1.0f, 0.0f, 0.0f});
    model = glm::rotate(model, glm::radians(this->rotation.y), {0.0f, 1.0f, 0.0f});

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



  /** Convert a vec4 from world space to local space */
  glm::vec3 worldToLocal(glm::vec4 vec)
  {
    return glm::inverse(this->getModelMatrix()) * vec;
  };


  /** Convert a vec4 from local space to world space */
  glm::vec3 localToWorld(glm::vec4 vec)
  {
    return this->getModelMatrix() * vec;
  };


};