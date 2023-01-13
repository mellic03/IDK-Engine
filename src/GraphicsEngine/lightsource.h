#pragma once

#include "../transform.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"


struct VolumetricData {
  int num_samples = 128;
  float step_size = 0.05;
  float step_multiplier = 1.01;
  
  int resolution_divisor = 2;
  int num_blur_passes = 2;

  float texel_size = 1.0f;
  float x_strength = 1.0f;
  float y_strength = 1.0f;
};

struct BloomData {
  int num_blur_passes = 5;

  float texel_size = 1.0f;
  float x_strength = 1.0f;
  float y_strength = 1.0f;
};

struct DirLight {
  glm::vec3 position = glm::vec3(10.0f, 15.0f, 10.0f);
  glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
  glm::vec3 diffuse = glm::vec3(0.82f, 0.69f, 0.37f);
  glm::vec3 ambient = glm::vec3(0.26f, 0.26f, 0.26f);
  float bias = 0.15f;
  float fog_intensity = 0.005f;
};

struct PointLight {
  
  Transform default_transform;

  GLuint FBO, depthCubemap;
  bool shadowmapped = false;
  bool active = false, volumetrics_active = false;
  Transform *m_transform = &this->default_transform;
  glm::vec3 ambient = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  float radius = 25.0f;
  float constant = 1.0f;
  float linear = 0.09f;
  float quadratic = 0.032f;
  float bias = 0.15f;
  float fog_constant = 1.0f;
  float fog_linear = 0.0f;
  float fog_quadratic = 1.0f;
  float fog_intensity = 0.01f;
};

struct SpotLight {
  
  private:
    Transform _default_transform;

  public:
    bool active = false;
    Transform *m_transform = &this->_default_transform;
    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float inner_cutoff = 12.5f;
    float outer_cutoff = 20.5f;
    float intensity = 1.0f;
    float fov = 60.0f;
    bool follow = true;
};