#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"

#include "texture.h"


struct Material {
  Texture diffuseMap, specularMap, emissionMap, normalMap;
  glm::vec3 lightsourceEmission = glm::vec3(0.0f);
  float spec_exponent = 64.0f;
};


