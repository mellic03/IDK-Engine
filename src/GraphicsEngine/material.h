#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "texture.h"


struct Material {
  glm::vec3 ambient = glm::vec3(0.0f);
  Texture diffuse, specular, emission, normal;
  glm::vec3 diffuse_color = glm::vec3(0.0f);
  float spec_exponent = 0.5f;
};


