#include "renderutil.h"


void RenderUtil::genShadowMap_cascade(GLuint *FBO, GLuint depthMaps[], size_t num_cascades, int w, int h)
{

  GLCALL( glDeleteFramebuffers(1, FBO) );
  GLCALL( glDeleteTextures(num_cascades, depthMaps) );

  GLCALL( glGenFramebuffers(1, FBO) );
  GLCALL( glGenTextures(num_cascades, depthMaps) );

  for (size_t i=0; i<num_cascades; i++)
  {
    glBindTexture(GL_TEXTURE_2D, depthMaps[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaps[0], 0);

  // Disable writes to the color buffer
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (Status != GL_FRAMEBUFFER_COMPLETE)
  {
    printf("[RenderUtil::genShadowMap_cascade()] FB error, status: 0x%x\n", Status);
    exit(1);
  }
}



void RenderUtil::bindWrite_cascade(GLuint FBO, GLuint depthMaps[], int index)
{
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaps[index], 0);
}



std::vector<glm::vec4> RenderUtil::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
  const auto inv = glm::inverse(proj * view);
  
  std::vector<glm::vec4> frustumCorners;
  for (unsigned int x = 0; x < 2; ++x)
  {
    for (unsigned int y = 0; y < 2; ++y)
    {
      for (unsigned int z = 0; z < 2; ++z)
      {
        const glm::vec4 pt = inv * glm::vec4(2.0f*x - 1.0f,  2.0f*y - 1.0f,  2.0f*z - 1.0f,  1.0f);
        frustumCorners.push_back(pt / pt.w);
      }
    }
  }
  
  return frustumCorners;
}


glm::mat4 RenderUtil::getLightSpaceMatrix_cascade(glm::mat4 proj, glm::mat4 view, glm::vec3 lightdir)
{
  std::vector<glm::vec4> corners = RenderUtil::getFrustumCornersWorldSpace(proj, view);

  glm::vec3 center = glm::vec3(0, 0, 0);
  for (const glm::vec4 &v: corners)
    center += glm::vec3(v);
  center /= corners.size();


  const glm::mat4 lightView = glm::lookAt(
    center + lightdir,
    center,
    glm::vec3(0.0f, 1.0f, 0.0f)
  );


  float minX = (lightView * corners[0]).x;
  float maxX = (lightView * corners[0]).x;
  float minY = (lightView * corners[0]).y;
  float maxY = (lightView * corners[0]).y;
  float minZ = (lightView * corners[0]).z;
  float maxZ = (lightView * corners[0]).z;

  for (const auto& v : corners)
  {
    const auto trf = lightView * v;
    minX = glm::min(minX, trf.x);
    maxX = glm::max(maxX, trf.x);
    minY = glm::min(minY, trf.y);
    maxY = glm::max(maxY, trf.y);
    minZ = glm::min(minZ, trf.z);
    maxZ = glm::max(maxZ, trf.z);
  }


  constexpr float zMult = 2.0f;
  if (minZ < 0)
    minZ *= zMult;
  else
    minZ /= zMult;

  if (maxZ < 0)
    maxZ /= zMult;
  else
    maxZ *= zMult;


  const glm::vec2 bounds = glm::vec2((maxX - minX), (maxY - minY)) / (2048.0f);

  glm::vec2 xx = glm::vec2(minX, maxX);
  glm::vec2 yy = glm::vec2(minY, maxY);

  xx /= bounds.x;
  xx = glm::floor(xx);
  xx *= bounds.x;

  yy /= bounds.y;
  yy = glm::floor(yy);
  yy *= bounds.y;

  const glm::mat4 lightProjection = glm::ortho(xx[0], xx[1], yy[0], yy[1], minZ, maxZ);

  return lightProjection * lightView;
}