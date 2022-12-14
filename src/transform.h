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
  }
};