#pragma once

#include "../transform.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

struct DirLight {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
  glm::vec3 ambient = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);
};

struct PointLight {
  Transform *m_transform;
  glm::vec3 ambient = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);
  float constant = 1.0f;
  float linear = 0.09f;
  float quadratic = 0.032f;
  float bias = 0.15f;
};

struct SpotLight {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
  glm::vec3 ambient = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);
  float constant = 1.0f;
  float linear = 0.09f;
  float quadratic = 0.032f;
  float inner_cutoff = 12.5f;
  float outer_cutoff = 20.5f;
  float intensity = 1.0f;
  float fov = 60.0f;
  bool follow = true;
};