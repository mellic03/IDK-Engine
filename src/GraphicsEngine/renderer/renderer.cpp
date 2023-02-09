#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include "renderer.h"

#include "../../scene/scene.h"


void Renderer::createShader(std::string filename, ShaderType type)
{
  std::string filepath = "assets/shaders/" + filename;

  ShaderSource src = parse_shader(filepath + ".vs", filepath + ".fs", "NULL");
  Shader shader;
  shader.set(create_shader(src.vertex_source, src.fragment_source, src.geometry_source));
  shader.mapUniformLocs(); 
  this->shaders[type] = shader;
}


void Renderer::compileShaders(void)
{
  this->createShader("gbuffer_geometrypass/background",   SHADER_BACKGROUND);
  this->createShader("gbuffer_geometrypass/terrain",      SHADER_TERRAIN);

  this->createShader("gbuffer_geometrypass/billboard_fixed",    SHADER_BILLBOARD_FIXED);
  this->createShader("gbuffer_geometrypass/billboard_follow",   SHADER_BILLBOARD_FOLLOW);

  this->createShader("gbuffer_geometrypass/actor",           SHADER_ACTOR);
  this->createShader("gbuffer_geometrypass/actor_animated",  SHADER_ACTOR_ANIMATED);
  this->createShader("lightsource",                       SHADER_LIGHTSOURCE);


  this->createShader("screenquad",            SHADER_SCREENQUAD);
  this->createShader("volumetriclights",      SHADER_VOLUMETRIC_LIGHT);
  this->createShader("blur/blur_downsample",  SHADER_BLUR_DOWNSAMPLE);
  this->createShader("blur/blur_upsample",    SHADER_BLUR_UPSAMPLE);
  this->createShader("additive",              SHADER_ADDITIVE);


  this->createShader("gbuffer_lightingpass/gbuffer_lighting",      SHADER_GBUFFER_LIGHTING);
  this->createShader("fxaa",                  SHADER_FXAA);


  this->createShader("debug/wireframe",       SHADER_WIREFRAME);


  // Shadows
  //------------------------------------------------------
  this->createShader("shadowmap/dirshadow",             SHADER_DIRSHADOW);
  this->createShader("shadowmap/dirshadow_animated",    SHADER_DIRSHADOW_ANIMATED);



  ShaderSource pointshadow_src = parse_shader("assets/shaders/shadowmap/pointshadow.vs", "assets/shaders/shadowmap/pointshadow.fs", "assets/shaders/shadowmap/pointshadow.gs");
  Shader pointshadow;
  pointshadow.set(create_shader(pointshadow_src.vertex_source, pointshadow_src.fragment_source, pointshadow_src.geometry_source));
  pointshadow.mapUniformLocs();
  this->shaders[SHADER_POINTSHADOW] = pointshadow;


  pointshadow_src = parse_shader("assets/shaders/shadowmap/pointshadow_animated.vs", "assets/shaders/shadowmap/pointshadow_animated.fs", "assets/shaders/shadowmap/pointshadow_animated.gs");
  pointshadow;
  pointshadow.set(create_shader(pointshadow_src.vertex_source, pointshadow_src.fragment_source, pointshadow_src.geometry_source));
  pointshadow.mapUniformLocs();
  this->shaders[SHADER_POINTSHADOW_ANIMATED] = pointshadow;
  //------------------------------------------------------

}


void Renderer::init(void)
{
  this->compileShaders();

  // Load primitive models
  //------------------------------------------------------
  this->_primitives.sphere_primitive.model.loadDae("src/GraphicsEngine/renderer/primitives/", "sphere.dae", false);
  this->_primitives.box_primitive.model.loadDae("src/GraphicsEngine/renderer/primitives/", "cube.dae", false);
  //------------------------------------------------------


  // Generate screen quad
  //------------------------------------------------------
  GLCALL(glGenVertexArrays(1, &this->quadVAO));
  GLCALL(glGenBuffers(1, &this->quadVBO));
  GLCALL(glBindVertexArray(this->quadVAO));
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO));
  glBufferData(GL_ARRAY_BUFFER, sizeof(this->quadVertices), &this->quadVertices, GL_STATIC_DRAW);

  GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
  GLCALL(glEnableVertexAttribArray(0));
  GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
  GLCALL(glEnableVertexAttribArray(2));
  //------------------------------------------------------


  // Create colour framebuffer
  //------------------------------------------------------
  this->genColorBuffer(2560, 2560);
  this->genVolLightBuffer(2560, 2560);
  this->genScreenQuadBuffer(2560, 2560);
  //------------------------------------------------------


  // Directional light depthmap
  //------------------------------------------------------
  
  RenderUtil::genShadowMap_cascade(&this->dirlight_depthmapFBO_cascade, this->dirlight_depthmapArray, NUM_SHADOW_CASCADES, 2048, 2048);



  glGenFramebuffers(1, &this->dirlight_depthmapFBO); 
  glGenTextures(1, &this->dirlight_depthmap);
  glBindTexture(GL_TEXTURE_2D, this->dirlight_depthmap);
  GLCALL( glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL) );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindFramebuffer(GL_FRAMEBUFFER, this->dirlight_depthmapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->dirlight_depthmap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //------------------------------------------------------


  this->resize(this->viewport_width, this->viewport_height);
}


void Renderer::genDepthCubemap(GLuint *FBO, GLuint *texture)
{
  glDeleteFramebuffers(1, FBO); 
  glDeleteTextures(1, texture);

  glGenFramebuffers(1, FBO); 
  glGenTextures(1, texture);

  glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
  for (unsigned int j = 0; j < 6; ++j)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *texture, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::useShader(ShaderType shader)
{
  this->active_shader = &this->shaders[shader];
  GLCALL(glUseProgram(this->active_shader->programID));
}


void Renderer::useShader(Shader *shader)
{
  this->active_shader = shader;
  GLCALL(glUseProgram(this->active_shader->programID));
}


void Renderer::postProcess(void)
{
  this->active_shader->setFloatVector("kernel", 9, this->image_kernel);
  this->active_shader->setFloat("kernelDivisor", this->kernel_divisor);
  this->active_shader->setFloat("kernelOffsetDivisor", this->kernel_offset_divisor);
  this->active_shader->setFloat("exposure", this->exposure);
  this->active_shader->setFloat("gamma", this->gamma);
}


#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


void Renderer::setupDirLightDepthmap(glm::vec3 dirlightpos, glm::vec3 dirlightdir)
{
  this->lightSpaceMatrices_lastFrame = this->lightSpaceMatrices;
  if (this->lightSpaceMatrices_lastFrame.size() == 0)
  {
    for (int i=0; i<NUM_SHADOW_CASCADES; i++)
      this->lightSpaceMatrices_lastFrame.push_back(glm::mat4(1.0f));
  }

  this->lightSpaceMatrices.clear();

  glm::mat4 proj = glm::perspective(
    this->cam.fov,
    (float)this->viewport_width / (float)this->viewport_height,
    this->cam.near,
    this->shadow_cascades[0] + 5.0f
  );
  this->lightSpaceMatrices.push_back(RenderUtil::getLightSpaceMatrix_cascade(proj, this->cam.view, this->lightSpaceMatrices_lastFrame[0], glm::normalize(dirlightpos)));


  for (size_t i=1; i<NUM_SHADOW_CASCADES; i++)
  {
    proj = glm::perspective(
      this->cam.fov,
      (float)this->viewport_width / (float)this->viewport_height,
      this->shadow_cascades[i-1] - 5.0f,
      this->shadow_cascades[i] + 5.0f
    );
   
    this->lightSpaceMatrices.push_back(RenderUtil::getLightSpaceMatrix_cascade(proj, this->cam.view, this->lightSpaceMatrices_lastFrame[i], glm::normalize(dirlightpos)));
  }

  this->lightSpaceMatrix = this->lightSpaceMatrices[0];
  this->active_shader->setMat4("lightSpaceMatrix", this->lightSpaceMatrix);


  this->lightSpaceMatrices_lastFrame.clear();
}


void Renderer::perFrameUpdate(void)
{
  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->viewport_width/(float)this->viewport_height, this->near_plane, this->far_plane);
  this->cam.aspect = (float)this->viewport_width/(float)this->viewport_height;
  this->cam.near = this->near_plane;
  this->cam.far = this->far_plane;
  this->_frustum.update(&this->cam, this->cam.aspect, glm::radians(this->fov), this->near_plane, this->far_plane);

  for (int i=0; i<SHADER_NUM_SHADERS; i++)
  {
    this->useShader(&this->shaders[i]);
    this->shaders[i].setMat4("projection", this->cam.projection);
    this->shaders[i].setMat4("view", this->cam.view);
  }
}


void Renderer::copyTexture(GLuint src, GLuint dest)
{
  glCopyImageSubData( src,  GL_TEXTURE_2D, 0, 0, 0, 0,
                      dest, GL_TEXTURE_2D, 0, 0, 0, 0,
                      this->viewport_width, this->viewport_height, 1);
}


void Renderer::additiveBlend(GLuint texture_1, GLuint texture_2)
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->generalFBO);
  this->useShader(SHADER_ADDITIVE);

  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_2D, texture_1);
  this->active_shader->setInt("texture_1", 10); 

  glActiveTexture(GL_TEXTURE11);
  glBindTexture(GL_TEXTURE_2D, texture_2);
  this->active_shader->setInt("texture_2", 11); 


  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(this->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);


  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::blurTexture(GLuint input_texture, GLuint output_framebuffer)
{
  GLCALL( glDisable(GL_DEPTH_TEST); );

  this->useShader(SHADER_BLUR_DOWNSAMPLE);
  
  glActiveTexture(GL_TEXTURE0);
  this->active_shader->setInt("inputTexture", 0);
  

  glBindVertexArray(this->quadVAO);


  // Downsampling
  //--------------------------------------------------------------------------
  int i = 0;
  for (i=0; i<this->num_blur_FBOs; i++)
  {
    glViewport(0, 0, this->viewport_width/pow(2, i+1), this->viewport_height/pow(2, i+1));
    glBindFramebuffer(GL_FRAMEBUFFER, this->blurFBOS[i]);
    
    if (i == 0)
      glBindTexture(GL_TEXTURE_2D, input_texture);
    else
      glBindTexture(GL_TEXTURE_2D, this->blurColorBuffers[i-1]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
  //--------------------------------------------------------------------------

  // Upsampling
  // --------------------------------------------------------------------------
  this->useShader(SHADER_BLUR_UPSAMPLE);
  glViewport(0, 0, this->viewport_width, this->viewport_height);

  for (i=this->num_blur_FBOs-1; i>=0; i--)
  {
    glViewport(0, 0, this->viewport_width/pow(2, i), this->viewport_height/pow(2, i));

    if (i == 0)
      glBindFramebuffer(GL_FRAMEBUFFER, output_framebuffer);
    else
      glBindFramebuffer(GL_FRAMEBUFFER, this->blurFBOS[i-1]);


    glActiveTexture(GL_TEXTURE0);
    this->active_shader->setInt("inputTexture0", 0);
    glBindTexture(GL_TEXTURE_2D, this->blurColorBuffers[i]);
    glBindTexture(GL_TEXTURE_2D, this->blurColorBuffers[i]);


    glActiveTexture(GL_TEXTURE1);
    this->active_shader->setInt("inputTexture1", 1);
    if (i == this->num_blur_FBOs-1)
      glBindTexture(GL_TEXTURE_2D, this->blurColorBuffers[i]);
    else
      glBindTexture(GL_TEXTURE_2D, this->blurColorBuffers[i+1]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
  // --------------------------------------------------------------------------

  glBindVertexArray(0);
  GLCALL( glEnable(GL_DEPTH_TEST); );
}


void Renderer::genBlurBuffers(int x, int y)
{
  glDeleteTextures(MAX_BLUR_FBOS, this->blurColorBuffers);
  glDeleteFramebuffers(MAX_BLUR_FBOS, this->blurFBOS);

  glGenFramebuffers(MAX_BLUR_FBOS, this->blurFBOS);
  glGenTextures(MAX_BLUR_FBOS, this->blurColorBuffers);
  
  for (int i=0; i<MAX_BLUR_FBOS; i++)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, this->blurFBOS[i]);

    glBindTexture(GL_TEXTURE_2D, this->blurColorBuffers[i]);

    int res_x = x / pow(2, i+1);
    int res_y = y / pow(2, i+1);

    if (res_x <= this->bloom.min_downsample_resolution || res_y <= this->bloom.min_downsample_resolution)
    {
      this->num_blur_FBOs = i;
      break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, res_x, res_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, this->blurColorBuffers[i], 0);

    GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);
  }


  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genGBuffer(int x, int y)
{
  glDeleteTextures(1, &this->gbuffer_position);
  glDeleteTextures(1, &this->gbuffer_normal);
  glDeleteTextures(1, &this->gbuffer_albedospec);
  glDeleteTextures(1, &this->gbuffer_emission);

  glDeleteRenderbuffers(1, &this->gbufferRBO);
  glDeleteFramebuffers(1, &this->gbufferFBO);

  glGenRenderbuffers(1, &this->gbufferRBO);
  glGenFramebuffers(1, &this->gbufferFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->gbufferFBO);
  

  auto genBufferLambda = [](GLuint *texture, int w, int h, int attachment)
  {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, *texture, 0);
  };

  genBufferLambda(&this->gbuffer_position, x, y, 0);
  genBufferLambda(&this->gbuffer_normal, x, y, 1);
  genBufferLambda(&this->gbuffer_albedospec, x, y, 2);
  genBufferLambda(&this->gbuffer_emission, x, y, 3);

  // // - position color buffer
  // glGenTextures(1, &this->gbuffer_position);
  // glBindTexture(GL_TEXTURE_2D, this->gbuffer_position);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_FLOAT, NULL);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gbuffer_position, 0);
    
  // // - normal color buffer
  // glGenTextures(1, &this->gbuffer_normal);
  // glBindTexture(GL_TEXTURE_2D, this->gbuffer_normal);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_FLOAT, NULL);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gbuffer_normal, 0);

  // // - color + specular color buffer
  // glGenTextures(1, &this->gbuffer_albedospec);
  // glBindTexture(GL_TEXTURE_2D, this->gbuffer_albedospec);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->gbuffer_albedospec, 0);

  // // - emission color buffer
  // glGenTextures(1, &this->gbuffer_emission);
  // glBindTexture(GL_TEXTURE_2D, this->gbuffer_emission);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->gbuffer_emission, 0);
    

  GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
  glDrawBuffers(4, attachments);

  glBindRenderbuffer(GL_RENDERBUFFER, this->gbufferRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, x, y);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->gbufferRBO);


  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genGeneralBuffer(int x, int y)
{
  glDeleteTextures(2, this->generalColorBuffers);
  glDeleteFramebuffers(1, &this->generalFBO);

  glGenFramebuffers(1, &this->generalFBO);
  glGenTextures(2, this->generalColorBuffers);
  
  glBindFramebuffer(GL_FRAMEBUFFER, this->generalFBO);

  for (int i=0; i<2; i++)
  {
    glBindTexture(GL_TEXTURE_2D, this->generalColorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->generalColorBuffers[i], 0);
  }

  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genBillboardBuffer(int x, int y)
{
  glDeleteTextures(1, &this->billboardColorBuffer);
  glDeleteRenderbuffers(1, &this->billboardRBO);
  glDeleteFramebuffers(1, &this->billboardFBO);

  glGenFramebuffers(1, &this->billboardFBO);
  glGenRenderbuffers(1, &this->billboardRBO);
  glGenTextures(1, &this->billboardColorBuffer);
  
  glBindFramebuffer(GL_FRAMEBUFFER, this->billboardFBO);

  glBindTexture(GL_TEXTURE_2D, this->billboardColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, this->billboardColorBuffer, 0);

  GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glBindRenderbuffer(GL_RENDERBUFFER, this->billboardRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, x, y);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->billboardRBO);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genColorBuffer(int x, int y)
{
  glDeleteTextures(1, &this->colorBuffer);
  glDeleteRenderbuffers(1, &this->colorRBO);
  glDeleteFramebuffers(1, &this->colorFBO);

  glGenFramebuffers(1, &this->colorFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->colorFBO);
  glGenTextures(1, &this->colorBuffer);
  
  glBindTexture(GL_TEXTURE_2D, this->colorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorBuffer, 0);

  glGenRenderbuffers(1, &this->colorRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->colorRBO); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->colorRBO);


  GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genVolLightBuffer(int x, int y)
{
  glDeleteTextures(1, &this->lightshaftColorBuffer);
  glDeleteRenderbuffers(1, &this->colorRBO);
  glDeleteFramebuffers(1, &this->lightshaftFBO);

  glGenFramebuffers(1, &this->lightshaftFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->lightshaftFBO);
  glGenTextures(1, &this->lightshaftColorBuffer);

  glBindTexture(GL_TEXTURE_2D, this->lightshaftColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, this->lightshaftColorBuffer, 0);


  glGenRenderbuffers(1, &this->colorRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->colorRBO); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->colorRBO);


  GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genScreenQuadBuffer(int x, int y)
{
  glDeleteTextures(1, &this->screenQuadColorBuffers[0]);
  glDeleteFramebuffers(1, &this->screenQuadFBO);

  glGenFramebuffers(1, &this->screenQuadFBO);
  glGenTextures(1, &this->screenQuadColorBuffers[0]);
  
  glBindFramebuffer(GL_FRAMEBUFFER, this->screenQuadFBO);

  glBindTexture(GL_TEXTURE_2D, this->screenQuadColorBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, this->screenQuadColorBuffers[0], 0);

  GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genPingPongBuffer(int x, int y)
{
  glDeleteFramebuffers(2, this->pingPongFBO);
  glDeleteTextures(2, this->pingPongColorBuffers);

  glGenFramebuffers(2, this->pingPongFBO);
  glGenTextures(2, this->pingPongColorBuffers);

  for (unsigned int i = 0; i < 2; i++)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, this->pingPongFBO[i]);
    glBindTexture(GL_TEXTURE_2D, this->pingPongColorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->pingPongColorBuffers[i], 0);

    GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}


void Renderer::resize(int x, int y)
{
  this->genBlurBuffers(x, y);

  this->genGBuffer(x, y);
  this->genGeneralBuffer(x, y);
  this->genBillboardBuffer(x, y);
  this->genColorBuffer(x, y);
  this->genVolLightBuffer(x/this->volumetrics.resolution_divisor, y/this->volumetrics.resolution_divisor);
  this->genPingPongBuffer(x, y);
  this->genScreenQuadBuffer(x, y);

  // RenderUtil::genShadowMap_cascade(&this->dirlight_depthmapFBO_cascade, this->dirlight_depthmapArray, NUM_SHADOW_CASCADES, x/2, y/2);

  this->viewport_width = x;
  this->viewport_height = y;

  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->viewport_width/(float)this->viewport_height, this->near_plane, this->far_plane);
}


void unbindTextureUnit(GLenum texture_unit)
{
  GLCALL( glActiveTexture(texture_unit) );
  GLCALL( glBindTexture(GL_TEXTURE_2D, 0) );
}



void Renderer::drawPrimitive(PrimitiveType type, glm::vec3 pos, float radius, Transform *transform)
{
  GLCALL( glDisable(GL_CULL_FACE) );

  Transform localtransform;
  localtransform.position = pos;
  localtransform.scale = glm::vec3(radius);

  this->active_shader->setMat4("model", localtransform.getModelMatrix());

  Mesh *mesh = &this->_primitives.sphere_primitive.model.mesh;

  glBindVertexArray(mesh->VAO);
  for (size_t i=0; i<mesh->IBOS.size(); i++)
  {
    mesh->materials[i].diffuseMap.bind(  GL_TEXTURE0 );
    mesh->materials[i].specularMap.bind( GL_TEXTURE1 );
    mesh->materials[i].normalMap.bind(   GL_TEXTURE2 );
    mesh->materials[i].emissionMap.bind( GL_TEXTURE3 );

    this->active_shader->setFloat("material.spec_exponent", mesh->materials[i].spec_exponent);

    GLCALL( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
    GLCALL( glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size()) );
    GLCALL( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );

  }
  glBindVertexArray(0);


  GLCALL( glEnable(GL_CULL_FACE) );
}


void Renderer::drawPrimitive_box(Transform *transform)
{
  GLCALL( glDisable(GL_CULL_FACE) );

  this->active_shader->setMat4("model", transform->getModelMatrix());

  Mesh *mesh = &this->_primitives.box_primitive.model.mesh;

  glBindVertexArray(mesh->VAO);
  for (size_t i=0; i<mesh->IBOS.size(); i++)
  {
    mesh->materials[i].diffuseMap.bind(  GL_TEXTURE0 );
    mesh->materials[i].specularMap.bind( GL_TEXTURE1 );
    mesh->materials[i].normalMap.bind(   GL_TEXTURE2 );
    mesh->materials[i].emissionMap.bind( GL_TEXTURE3 );

    this->active_shader->setFloat("material.spec_exponent", mesh->materials[i].spec_exponent);

    GLCALL( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
    GLCALL( glDrawElements(GL_TRIANGLES, mesh->indices[i].size(), GL_UNSIGNED_INT, (void *)0) );
    GLCALL( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );

  }
  glBindVertexArray(0);

  GLCALL( glEnable(GL_CULL_FACE) );
}



void Renderer::drawModel(Model *model, Transform *transform)
{
  this->active_shader->setMat4("model", transform->getModelMatrix_stale());

  Mesh &mesh = model->mesh;

  GLCALL( glBindVertexArray(mesh.VAO) );

  for (size_t i=0; i<mesh.IBOS.size(); i++)
  {
    mesh.materials[i].diffuseMap.bind(  GL_TEXTURE0 );
    mesh.materials[i].specularMap.bind( GL_TEXTURE1 );
    mesh.materials[i].normalMap.bind(   GL_TEXTURE2 );
    mesh.materials[i].emissionMap.bind( GL_TEXTURE3 );

    this->active_shader->setFloat("material.spec_exponent", mesh.materials[i].spec_exponent);

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]));
    GLCALL(glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0));

    unbindTextureUnit(GL_TEXTURE0);
    unbindTextureUnit(GL_TEXTURE1);
    unbindTextureUnit(GL_TEXTURE2);
    unbindTextureUnit(GL_TEXTURE3);
  }

  GLCALL( glBindVertexArray(0) );
}


void Renderer::drawModel_shadowtest(Model *model, Transform *transform)
{
  this->active_shader->setMat4("model", transform->getModelMatrix_stale());

  Mesh &mesh = model->mesh;

  GLCALL( glBindVertexArray(mesh.shadowVAO) );

  for (size_t i=0; i<mesh.IBOS.size(); i++)
  {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]));
    GLCALL(glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0));
  }

  GLCALL( glBindVertexArray(0) );
}




void Renderer::drawModelAnimated(Model *model, Transform *transform, Animation::AnimationController *animationController)
{
  for (int i=0; i<Animation::MAX_BONES; i++)
    this->active_shader->setMat4("boneTransforms[" + std::to_string(i) + "]", glm::mat4(1.0f));

  Animation::Animation *anim1 = animationController->getAnimation();
  Animation::Armature *armature1 = anim1->getArmature();

  for (size_t i=0; i<armature1->joints.size(); i++)
  {
    glm::mat4 boneTransform = armature1->joints[i]->finalTransform;
    this->active_shader->setMat4("boneTransforms[" + std::to_string(i) + "]", boneTransform);
  }
  this->drawModel(model, transform);

  GLCALL( glBindVertexArray(0) );
}




void Renderer::drawModelAnimated_blend(Model *model, Transform *transform, Animation::Animation *anim1, Animation::Animation *anim2, float alpha)
{
  for (int i=0; i<Animation::MAX_BONES; i++)
    this->active_shader->setMat4("boneTransforms[" + std::to_string(i) + "]", glm::mat4(1.0f));

  Animation::Armature *armature = anim1->getArmature();
  armature->computePose_blended(anim1->getTime(), anim2->getArmature(), alpha);


  for (size_t i=0; i<armature->joints.size(); i++)
  {
    glm::mat4 boneTransform = armature->joints[i]->finalTransform;

    this->active_shader->setMat4("boneTransforms[" + std::to_string(i) + "]", boneTransform);
  }
  this->drawModel(model, transform);


  anim1->advance(this->deltaTime);
  anim2->advance(this->deltaTime);
}



void Renderer::drawLightSource(Model *model, Transform *transform, glm::vec3 diffuse)
{
  this->active_shader->setMat4("model", transform->getModelMatrix_stale());

  Mesh &mesh = model->mesh;

  glBindVertexArray(mesh.VAO);

  for (size_t i=0; i<mesh.IBOS.size(); i++)
  {
    this->active_shader->setVec3("diffuseColor", diffuse);

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]));
    GLCALL(glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0));
  }

  glBindVertexArray(0);
}


void Renderer::drawTerrain(Model *model, Transform *transform, float threshold, float epsilon)
{
  this->active_shader->setMat4("model", transform->getModelMatrix_stale());

  this->active_shader->setFloat("threshold", threshold);
  this->active_shader->setFloat("epsilon",   epsilon);

  this->shaders[SHADER_TERRAIN].setInt("material.diffuseMap1", 0);
  this->shaders[SHADER_TERRAIN].setInt("material.diffuseMap2", 1);

  this->shaders[SHADER_TERRAIN].setInt("material.specularMap1", 2);
  this->shaders[SHADER_TERRAIN].setInt("material.specularMap2", 3);

  this->shaders[SHADER_TERRAIN].setInt("material.normalMap1", 4);
  this->shaders[SHADER_TERRAIN].setInt("material.normalMap2", 5);



  Mesh &mesh = model->mesh;

  GLCALL(glBindVertexArray(mesh.VAO));

  model->materials[0].diffuseMap.bind(  GL_TEXTURE0 );
  model->materials[1].diffuseMap.bind(  GL_TEXTURE1 );

  model->materials[0].specularMap.bind( GL_TEXTURE2 );
  model->materials[1].specularMap.bind( GL_TEXTURE3 );

  model->materials[0].normalMap.bind(   GL_TEXTURE4 );
  model->materials[1].normalMap.bind(   GL_TEXTURE5 );


  for (size_t i=0; i<mesh.IBOS.size(); i++)
  {
    this->active_shader->setFloat("material.spec_exponent", mesh.materials[i].spec_exponent);

    GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]) );
    GLCALL( glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0) );
  }

  glBindVertexArray(0);
}


void Renderer::drawBillboard(Model *model, Transform *transform)
{
  this->active_shader->setMat4("model", transform->getModelMatrix_stale());

  GLCALL( glActiveTexture(GL_TEXTURE0) );
  this->active_shader->setInt("diffuseMap", 0);

  Mesh &mesh = model->mesh;
  GLCALL(glBindVertexArray(mesh.VAO));

  for (size_t i=0; i<mesh.IBOS.size(); i++)
  {
    GLCALL( glBindTexture(GL_TEXTURE_2D, mesh.materials[i].diffuseMap.m_texture_obj) );

    GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]) );
    GLCALL( glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0) );
  }
 
}


void Renderer::drawModelInstanced(Model *model, InstanceData *instance_data)
{
  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, instance_data->VBO) );
  GLCALL( glActiveTexture(GL_TEXTURE0) );
  
  Mesh &mesh = model->mesh;
  glBindVertexArray(mesh.VAO);

  for (size_t i=0; i<mesh.indices.size(); i++)
  {
    GLCALL( glBindTexture(GL_TEXTURE_2D, mesh.materials[i].diffuseMap.m_texture_obj) );

    GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]) );
    glDrawElementsInstanced(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, 0, instance_data->model_transforms.size());
  }
}






Renderer Render::ren;
