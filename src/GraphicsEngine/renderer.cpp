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

Renderer::Renderer()
{
  ShaderSource worldspace_src = parse_shader("assets/shaders/worldspace.vs", "assets/shaders/worldspace.fs", "NULL");
  Shader worldspace;
  worldspace.set(create_shader(worldspace_src.vertex_source, worldspace_src.fragment_source, worldspace_src.geometry_source));
  this->shaders[SHADER_WORLDSPACE] = worldspace;

  ShaderSource viewspace_src = parse_shader("assets/shaders/viewspace.vs", "assets/shaders/viewspace.fs", "NULL");
  Shader viewspace;
  viewspace.set(create_shader(viewspace_src.vertex_source, viewspace_src.fragment_source, viewspace_src.geometry_source));
  this->shaders[SHADER_VIEWSPACE] = viewspace;

  ShaderSource lightsource_src = parse_shader("assets/shaders/lightsource.vs", "assets/shaders/lightsource.fs", "NULL");
  Shader lightsource;
  lightsource.set(create_shader(lightsource_src.vertex_source, lightsource_src.fragment_source, lightsource_src.geometry_source));
  this->shaders[SHADER_LIGHTSOURCE] = lightsource;
  
  ShaderSource renderquad_src = parse_shader("assets/shaders/renderquad.vs", "assets/shaders/renderquad.fs", "NULL");
  Shader renderquad;
  renderquad.set(create_shader(renderquad_src.vertex_source, renderquad_src.fragment_source, renderquad_src.geometry_source));
  this->shaders[SHADER_RENDERQUAD] = renderquad;

  ShaderSource shadowmap_src = parse_shader("assets/shaders/shadowmap.vs", "assets/shaders/shadowmap.fs", "NULL");
  Shader shadowmap;
  shadowmap.set(create_shader(shadowmap_src.vertex_source, shadowmap_src.fragment_source, shadowmap_src.geometry_source));
  this->shaders[SHADER_SHADOWMAP] = shadowmap;

  ShaderSource testsrc = parse_shader("assets/shaders/renderquad.vs", "assets/shaders/guassianblur.fs", "NULL");
  Shader test;
  test.set(create_shader(testsrc.vertex_source, testsrc.fragment_source, testsrc.geometry_source));
  this->shaders[SHADER_TEST] = test;

  ShaderSource final = parse_shader("assets/shaders/renderquad.vs", "assets/shaders/renderquad.fs", "NULL");
  Shader fin;
  fin.set(create_shader(final.vertex_source, final.fragment_source, final.geometry_source));
  this->shaders[SHADER_FIN] = fin;


  // Generate screen quad
  //------------------------------------------------------
  glGenVertexArrays(1, &this->quadVAO);
  glGenBuffers(1, &this->quadVBO);
  glBindVertexArray(this->quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &this->quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  //------------------------------------------------------


  // Create colour framebuffer
  //------------------------------------------------------
  glGenFramebuffers(1, &this->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glGenTextures(2, this->colorBuffers);
  const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
  
  for (GLuint i=0; i<2; i++)
  {
    glBindTexture(GL_TEXTURE_2D, this->colorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->colorBuffers[i], 0);
  }



  glGenRenderbuffers(1, &this->rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, this->rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SHADOW_WIDTH, SHADOW_HEIGHT);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);
  //------------------------------------------------------


  // Create depth framebuffer
  //------------------------------------------------------
  glGenFramebuffers(1, &this->depthMapFBO); 

  glGenTextures(1, &this->depthMap);
  glBindTexture(GL_TEXTURE_2D, this->depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
              1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);  
  //------------------------------------------------------


  // Create depth cubemap
  //------------------------------------------------------
  // glGenTextures(1, &this->depthCubemap);
  // glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
  // for (unsigned int i = 0; i < 6; ++i)
  //   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
  // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubemap, 0);
  // glDrawBuffer(GL_NONE);
  // glReadBuffer(GL_NONE);
  // glBindFramebuffer(GL_FRAMEBUFFER, 0); 
  //------------------------------------------------------





  for (int i=0; i<NUM_DIRLIGHTS; i++)
    this->dirlights.push_back(DirLight());

  for (int i=0; i<NUM_POINTLIGHTS; i++)
    this->pointlights.push_back(PointLight());

  this->pointlights[0].diffuse  = {1.0, 1.0, 1.0};
  this->pointlights[0].specular = {0.15, 0.15, 0.15};
  this->pointlights[0].position = {1.0, 2.0, 1.0};

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    this->spotlights.push_back(SpotLight());
  this->spotlights[0].position = {-2.0f, 4.0f, -1.0f};


}

void Renderer::bindFrameBufferObject(GLuint buffer, int x, int y)
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);


}

void Renderer::bindTexture(GLuint texture, int x, int y)
{
  glBindTexture(GL_TEXTURE_2D, texture);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}


void Renderer::useShader(ShaderType shader)
{
  this->active_shader = this->shaders[shader];
  glUseProgram(this->active_shader.id);
}

void Renderer::postProcess(void)
{
  // this->active_shader.setInt("screenTexture", 0);
  this->active_shader.setFloatVector("kernel", 9, this->image_kernel);
  this->active_shader.setFloat("kernelDivisor", this->kernel_divisor);
  this->active_shader.setFloat("kernelOffsetDivisor", this->kernel_offset_divisor);
  this->active_shader.setFloat("exposure", this->exposure);
  this->active_shader.setFloat("gamma", this->gamma);
}

void Renderer::useOrthographic(float x, float y, float z)
{
  float near_plane = 1.0f, far_plane = 45.5;
  glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
  glm::mat4 lightView = glm::lookAt(  -25.0f * this->dirlights[0].direction,
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      glm::vec3( 0.0f, 1.0f,  0.0f) );

  this->lightSpaceMatrix = lightProjection * lightView;
  this->useShader(SHADER_SHADOWMAP);
  this->active_shader.setMat4("lightSpaceMatrix", this->lightSpaceMatrix);
}

void Renderer::usePerspective(void)
{
  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->SCR_width/(float)this->SCR_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
}


void Renderer::update(void)
{ 
  this->shaderready_pointlights.clear();
  this->shaderready_spotlights.clear();

  this->num_active_pointlights = 0;
  this->num_active_spotlights = 0;

  int count = 0;

  for (int i=0; i<NUM_POINTLIGHTS; i++)
    if (this->pointlights_on[i])
    {
      this->shaderready_pointlights.push_back(this->pointlights[i]);
      this->num_active_pointlights += 1;
    }
  
  for (int i=0; i<NUM_POINTLIGHTS; i++)
    if (!this->pointlights_on[i])
      this->shaderready_pointlights.push_back(this->pointlights[i]);
  

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    if (this->spotlights_on[i])
    {
      this->shaderready_spotlights.push_back(this->spotlights[i]);
      this->num_active_spotlights += 1;
    }

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    if (!this->spotlights_on[i])
      this->shaderready_spotlights.push_back(this->spotlights[i]);
}


void Renderer::sendLightsToShader(void)
{
  this->active_shader.setInt("num_active_pointlights", this->num_active_pointlights);
  this->active_shader.setInt("num_active_spotlights", this->num_active_spotlights);

  char buffer[64];

  for (int i=0; i<this->NM_DIRLIGHTS; i++)
  {
    sprintf(buffer, "dirlights[%d].direction", i);
    this->active_shader.setVec3(buffer,  this->dirlights[i].direction);

    sprintf(buffer, "dirlights[%d].ambient", i);
    this->active_shader.setVec3(buffer,  this->dirlights[i].ambient);

    sprintf(buffer, "dirlights[%d].diffuse", i);
    this->active_shader.setVec3(buffer,  this->dirlights[i].diffuse);

    sprintf(buffer, "dirlights[%d].specular", i);
    this->active_shader.setVec3(buffer,  this->dirlights[i].specular);
  }

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    sprintf(buffer, "pointlights[%d].position", i);
    this->active_shader.setVec3(buffer,  this->shaderready_pointlights[i].position);

    sprintf(buffer, "pointlights[%d].ambient", i);
    this->active_shader.setVec3(buffer,  this->shaderready_pointlights[i].ambient);

    sprintf(buffer, "pointlights[%d].diffuse", i);
    this->active_shader.setVec3(buffer,  this->shaderready_pointlights[i].diffuse);

    sprintf(buffer, "pointlights[%d].specular", i);
    this->active_shader.setVec3(buffer,  this->shaderready_pointlights[i].specular);

    sprintf(buffer, "pointlights[%d].constant", i);
    this->active_shader.setFloat(buffer,  this->shaderready_pointlights[i].constant); 

    sprintf(buffer, "pointlights[%d].linear", i);
    this->active_shader.setFloat(buffer,  this->shaderready_pointlights[i].linear); 

    sprintf(buffer, "pointlights[%d].quadratic", i);
    this->active_shader.setFloat(buffer,  this->shaderready_pointlights[i].quadratic); 
  }

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
  {
    sprintf(buffer, "spotlights[%d].position", i);
    this->active_shader.setVec3(buffer,  this->shaderready_spotlights[i].position);

    sprintf(buffer, "spotlights[%d].direction", i);
    this->active_shader.setVec3(buffer,  this->shaderready_spotlights[i].direction);

    sprintf(buffer, "spotlights[%d].ambient", i);
    this->active_shader.setVec3(buffer,  this->shaderready_spotlights[i].ambient);

    sprintf(buffer, "spotlights[%d].diffuse", i);
    this->active_shader.setVec3(buffer,  this->shaderready_spotlights[i].diffuse);

    sprintf(buffer, "spotlights[%d].specular", i);
    this->active_shader.setVec3(buffer,  this->shaderready_spotlights[i].specular);

    sprintf(buffer, "spotlights[%d].constant", i);
    this->active_shader.setFloat(buffer,  this->shaderready_spotlights[i].constant);

    sprintf(buffer, "spotlights[%d].linear", i);
    this->active_shader.setFloat(buffer,  this->shaderready_spotlights[i].linear);

    sprintf(buffer, "spotlights[%d].quadratic", i);
    this->active_shader.setFloat(buffer,  this->shaderready_spotlights[i].quadratic);

    sprintf(buffer, "spotlights[%d].inner_cutoff", i);
    this->active_shader.setFloat(buffer,  glm::cos(glm::radians(this->shaderready_spotlights[i].inner_cutoff)));

    sprintf(buffer, "spotlights[%d].outer_cutoff", i);
    this->active_shader.setFloat(buffer,  glm::cos(glm::radians(this->shaderready_spotlights[i].outer_cutoff)));

    sprintf(buffer, "spotlights[%d].intensity", i);
    this->active_shader.setFloat(buffer,  this->shaderready_spotlights[i].intensity);
  }


  this->active_shader.setVec3("viewPos", this->cam.pos);
}