#pragma once

#include "../glcall.h"
#include "../transform/transform.h"
#include "../lightsource.h"

#include <cstdio>
#include <cstdlib>

#include <vector>

namespace RenderUtil {

  void genShadowMap_cascade(GLuint *FBO, GLuint depthMaps[], size_t num_cascades, int w, int h);
  void bindWrite_cascade(ReflectiveShadowMapCascaded *rsm, int index);

  std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
  glm::mat4 getLightSpaceMatrix_cascade(glm::mat4 proj, glm::mat4 view, glm::vec3 lightdir);
};




