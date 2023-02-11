#include "lightsource.h"
#include "glcall.h"


void ReflectiveShadowMapCascaded::genBuffers()
{
  GLCALL( glDeleteFramebuffers(1, &this->FBO) );
  GLCALL( glDeleteTextures(NUM_SHADOW_CASCADES, this->depthArray) );
  GLCALL( glDeleteTextures(NUM_SHADOW_CASCADES, this->positionArray) );
  GLCALL( glDeleteTextures(NUM_SHADOW_CASCADES, this->normalArray) );
  GLCALL( glDeleteTextures(NUM_SHADOW_CASCADES, this->fluxArray) );

  GLCALL( glGenFramebuffers(1, &this->FBO) );
  GLCALL( glGenTextures(NUM_SHADOW_CASCADES, this->depthArray) );
  GLCALL( glGenTextures(NUM_SHADOW_CASCADES, this->positionArray) );
  GLCALL( glGenTextures(NUM_SHADOW_CASCADES, this->normalArray) );
  GLCALL( glGenTextures(NUM_SHADOW_CASCADES, this->fluxArray) );

  for (size_t i=0; i<NUM_SHADOW_CASCADES; i++)
  {
    auto genBuf = [](GLuint *texture, GLint internalFormat, GLenum format, GLenum attachment)
    {
      glBindTexture(GL_TEXTURE_2D, *texture);
      glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, 2048, 2048, 0, format, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *texture, 0);
    };

    genBuf(&this->depthArray[i], GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
    genBuf(&this->positionArray[i], GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0);
    genBuf(&this->normalArray[i], GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0);
    genBuf(&this->fluxArray[i], GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthArray[0], 0);

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

