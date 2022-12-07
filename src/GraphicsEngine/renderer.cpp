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

void Renderer::createShader(std::string filename, ShaderType type)
{
  std::string filepath = "assets/shaders/" + filename;

  ShaderSource src = parse_shader(filepath + ".vs", filepath + ".fs", "NULL");
  Shader shader;
  shader.set(create_shader(src.vertex_source, src.fragment_source, src.geometry_source));
  this->shaders[type] = shader;
}

Renderer::Renderer()
{
  this->createShader("worldspace",     SHADER_WORLDSPACE);
  this->createShader("viewspace",      SHADER_VIEWSPACE);
  this->createShader("lightsource",    SHADER_LIGHTSOURCE);
  this->createShader("screenquad",     SHADER_SCREENQUAD);

  ShaderSource pointshadow_src = parse_shader("assets/shaders/pointshadow.vs", "assets/shaders/pointshadow.fs", "assets/shaders/pointshadow.gs");
  Shader pointshadow;
  pointshadow.set(create_shader(pointshadow_src.vertex_source, pointshadow_src.fragment_source, pointshadow_src.geometry_source));
  this->shaders[SHADER_POINTSHADOW] = pointshadow;


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
  glGenTextures(1, this->colorBuffers);
  
  for (GLuint i=0; i<1; i++)
  {
    glBindTexture(GL_TEXTURE_2D, this->colorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2560, 2560, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->colorBuffers[i], 0);
  }



  glGenRenderbuffers(1, &this->rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, this->rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 2560, 2560);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

  GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);
  //------------------------------------------------------


  // Create depth cubemap
  //------------------------------------------------------
  glGenFramebuffers(1, &this->depthMapFBO); 

  glGenTextures(1, &this->depthCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthCubemap);
  for (unsigned int i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubemap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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


void Renderer::useShader(ShaderType shader)
{
  this->active_shader = &this->shaders[shader];
  glUseProgram(this->active_shader->id);
}

void Renderer::postProcess(void)
{
  // this->active_shader.setInt("screenTexture", 0);
  this->active_shader->setFloatVector("kernel", 9, this->image_kernel);
  this->active_shader->setFloat("kernelDivisor", this->kernel_divisor);
  this->active_shader->setFloat("kernelOffsetDivisor", this->kernel_offset_divisor);
  this->active_shader->setFloat("exposure", this->exposure);
  this->active_shader->setFloat("gamma", this->gamma);
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void Renderer::setupDepthCubemap(glm::vec3 pos, glm::vec3 dir)
{
  float aspect = (float)this->SHADOW_WIDTH / (float)this->SHADOW_HEIGHT;
  float near = 1.0f;
  float far = 25.0f;
  glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

  std::vector<glm::mat4> shadowTransforms;
  shadowTransforms.push_back(shadowProj * glm::lookAt(this->pointlights[0].position, this->pointlights[0].position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(this->pointlights[0].position, this->pointlights[0].position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(this->pointlights[0].position, this->pointlights[0].position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(this->pointlights[0].position, this->pointlights[0].position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(this->pointlights[0].position, this->pointlights[0].position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(this->pointlights[0].position, this->pointlights[0].position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));


  char buffer[64];
  for (int i=0; i<6; i++)
  {
    sprintf(buffer, "shadowMatrices[%d]", i);
    this->active_shader->setMat4(buffer, shadowTransforms[i]);
  }

  this->active_shader->setVec3("lightPos", this->pointlights[0].position);
  this->active_shader->setFloat("far_plane", far);

}

void Renderer::usePerspective(void)
{
  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->SCR_width/(float)this->SCR_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
}


void Renderer::update(glm::vec3 pos, glm::vec3 dir)
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
  

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
  {
    if (this->spotlights[i].follow)
    {
      this->spotlights[i].position = pos;
      this->spotlights[i].direction = dir;
    }
  }

}


void Renderer::sendLightsToShader(void)
{
  this->active_shader->setInt("num_active_pointlights", this->num_active_pointlights);
  this->active_shader->setInt("num_active_spotlights", this->num_active_spotlights);

  char buffer[64];

  // for (int i=0; i<this->NM_DIRLIGHTS; i++)
  // {
  //   sprintf(buffer, "dirlights[%d].direction", i);
  //   this->active_shader.setVec3(buffer,  this->dirlights[i].direction);

  //   sprintf(buffer, "dirlights[%d].ambient", i);
  //   this->active_shader.setVec3(buffer,  this->dirlights[i].ambient);

  //   sprintf(buffer, "dirlights[%d].diffuse", i);
  //   this->active_shader.setVec3(buffer,  this->dirlights[i].diffuse);

  //   sprintf(buffer, "dirlights[%d].specular", i);
  //   this->active_shader.setVec3(buffer,  this->dirlights[i].specular);
  // }

  // for (int i=0; i<NUM_POINTLIGHTS; i++)
  // {
  //   sprintf(buffer, "pointlights[%d].position", i);
  //   this->active_shader.setVec3(buffer,  this->pointlights[i].position);

  //   sprintf(buffer, "pointlights[%d].ambient", i);
  //   this->active_shader.setVec3(buffer,  this->pointlights[i].ambient);

  //   sprintf(buffer, "pointlights[%d].diffuse", i);
  //   this->active_shader.setVec3(buffer,  this->pointlights[i].diffuse);

  //   sprintf(buffer, "pointlights[%d].specular", i);
  //   this->active_shader.setVec3(buffer,  this->pointlights[i].specular);

  //   sprintf(buffer, "pointlights[%d].constant", i);
  //   this->active_shader.setFloat(buffer,  this->pointlights[i].constant); 

  //   sprintf(buffer, "pointlights[%d].linear", i);
  //   this->active_shader.setFloat(buffer,  this->pointlights[i].linear); 

  //   sprintf(buffer, "pointlights[%d].quadratic", i);
  //   this->active_shader.setFloat(buffer,  this->pointlights[i].quadratic); 
  // }

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
  {
    sprintf(buffer, "spotlights[%d].position", i);
    this->active_shader->setVec3(buffer,  this->shaderready_spotlights[i].position);

    sprintf(buffer, "spotlights[%d].direction", i);
    this->active_shader->setVec3(buffer,  this->shaderready_spotlights[i].direction);

    sprintf(buffer, "spotlights[%d].ambient", i);
    this->active_shader->setVec3(buffer,  this->shaderready_spotlights[i].ambient);

    sprintf(buffer, "spotlights[%d].diffuse", i);
    this->active_shader->setVec3(buffer,  this->shaderready_spotlights[i].diffuse);

    sprintf(buffer, "spotlights[%d].specular", i);
    this->active_shader->setVec3(buffer,  this->shaderready_spotlights[i].specular);

    sprintf(buffer, "spotlights[%d].constant", i);
    this->active_shader->setFloat(buffer,  this->shaderready_spotlights[i].constant);

    sprintf(buffer, "spotlights[%d].linear", i);
    this->active_shader->setFloat(buffer,  this->shaderready_spotlights[i].linear);

    sprintf(buffer, "spotlights[%d].quadratic", i);
    this->active_shader->setFloat(buffer,  this->shaderready_spotlights[i].quadratic);

    sprintf(buffer, "spotlights[%d].inner_cutoff", i);
    this->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->shaderready_spotlights[i].inner_cutoff)));

    sprintf(buffer, "spotlights[%d].outer_cutoff", i);
    this->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->shaderready_spotlights[i].outer_cutoff)));

    sprintf(buffer, "spotlights[%d].intensity", i);
    this->active_shader->setFloat(buffer,  this->shaderready_spotlights[i].intensity);
  }

  this->active_shader->setVec3("viewPos", this->cam.pos);
  this->active_shader->setVec3("viewDirection", this->cam.dir);
}