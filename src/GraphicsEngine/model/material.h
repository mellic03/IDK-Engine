#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"

#include "texture.h"


struct Material {
  Texture *diffuseMap, *specularMap, *emissionMap, *normalMap;
  float spec_exponent = 0.5f;
};


