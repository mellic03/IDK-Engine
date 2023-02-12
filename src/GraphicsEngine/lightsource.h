#pragma once

#include <vector>

#include "glcall.h"
#include "transform/transform.h"


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
  float threshold = 1.0f;
  float strength = 0.04f;
  int min_downsample_resolution = 32;
};


enum LightSourceType {
  LIGHTSOURCE_NONE,
  LIGHTSOURCE_DIRECTIONAL_LIGHT,
  LIGHTSOURCE_POINT_LIGHT,
  LIGHTSOURCE_SPOT_LIGHT
};


struct DirLight {
  glm::vec3 position = glm::vec3(10.0f, 15.0f, 10.0f);
  glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
  glm::vec3 diffuse = glm::vec3(1.0f);
  glm::vec3 ambient = glm::vec3(16.0/255.0);
  float bias = 0.0f;
  float fog_intensity = 0.001f;
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
  float bias = -0.04f;
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



#define NUM_SHADOW_CASCADES 4
#define DIRLIGHT_RES 2048

class ReflectiveShadowMapCascaded {

  public:
    GLuint FBO;
    GLuint depthArray[NUM_SHADOW_CASCADES];
    GLuint positionArray[NUM_SHADOW_CASCADES];
    GLuint normalArray[NUM_SHADOW_CASCADES];
    GLuint fluxArray[NUM_SHADOW_CASCADES];
    std::vector<glm::mat4> lightSpaceMatrices;

    std::vector<float> cascade_distances = { 16.0f, 64.0f, 128.0f, 256.0f, 512.0f, 1024.0f, 2048.0f, 4096.0f };

    void genBuffers();

};