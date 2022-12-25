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

}

void Renderer::init(void)
{
  this->createShader("terrain",        SHADER_TERRAIN);
  this->createShader("worldspace",     SHADER_WORLDSPACE);
  this->createShader("weapon",         SHADER_WEAPON);
  this->createShader("lightsource",    SHADER_LIGHTSOURCE);
  this->createShader("screenquad",     SHADER_SCREENQUAD);

  this->createShader("normals",        SHADER_NORMALS);


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
  glGenTextures(2, this->colorBuffers);
  
  for (GLuint i=0; i<2; i++)
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

  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);




  glGenFramebuffers(1, &this->screenFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->screenFBO);
  glGenTextures(1, this->screenColorBuffers);
  
  glBindTexture(GL_TEXTURE_2D, this->screenColorBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2560, 2560, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screenColorBuffers[0], 0);


  glGenRenderbuffers(1, &this->screenRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->screenRBO); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 2560, 2560);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->screenRBO);

  GLuint screenAttachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, screenAttachments);



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
  this->pointlights_on[0] = true;
  // this->pointlights[0].position = {1.0, 2.0, 1.0};

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
  glm::vec3 lightPos = *this->pointlights[0].m_transform->getPos();
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));


  char buffer[64];
  for (int i=0; i<6; i++)
  {
    sprintf(buffer, "shadowMatrices[%d]", i);
    this->active_shader->setMat4(buffer, shadowTransforms[i]);
  }

  this->active_shader->setVec3("lightPos", lightPos);
  this->active_shader->setFloat("far_plane", far);
}

void Renderer::usePerspective(void)
{
  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->viewport_width/(float)this->viewport_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
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

  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthCubemap);
    
  this->active_shader->setInt("depthMap", 10);
  this->active_shader->setFloat("far_plane",   25.0f);
  
  this->active_shader->setVec3( "pointlight.ambient", this->pointlights[0].ambient);
  this->active_shader->setVec3( "pointlight.diffuse", this->pointlights[0].diffuse);
  this->active_shader->setVec3( "pointlight.pos", this->pointlights[0].m_transform->getPos_worldspace());
  this->active_shader->setVec3( "pointlight.tangent_pos", *this->pointlights[0].m_transform->getPos());
  this->active_shader->setFloat("pointlight.constant", this->pointlights[0].constant);
  this->active_shader->setFloat("pointlight.linear", this->pointlights[0].linear);
  this->active_shader->setFloat("pointlight.quadratic", this->pointlights[0].quadratic);
  this->active_shader->setFloat("pointlight.bias", this->pointlights[0].bias);
  this->active_shader->setVec3( "clearColor", this->clearColor);
  this->active_shader->setFloat("fog_start", this->fog_start);
  this->active_shader->setFloat("fog_end", this->fog_end);



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

  this->active_shader->setVec3("viewPos", this->cam.m_transform->getPos_worldspace());
  this->active_shader->setVec3("viewDirection", glm::mat3(this->cam.modifier_matrix) * *this->cam.dir);
}

void Renderer::resize(int x, int y)
{
  glDeleteTextures(1, &this->colorBuffers[0]);
  glDeleteRenderbuffers(1, &this->rbo);
  glDeleteFramebuffers(1, &this->FBO);

  glDeleteTextures(1, &this->screenColorBuffers[0]);
  glDeleteRenderbuffers(1, &this->screenRBO);
  glDeleteFramebuffers(1, &this->screenFBO);

  glGenFramebuffers(1, &this->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glGenTextures(2, this->colorBuffers);
  
  for (GLuint i=0; i<1; i++)
  {
    glBindTexture(GL_TEXTURE_2D, this->colorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->colorBuffers[i], 0);
  }


  glGenRenderbuffers(1, &this->rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, this->rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);






  glGenFramebuffers(1, &this->screenFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->screenFBO);
  glGenTextures(1, this->screenColorBuffers);
  
  glBindTexture(GL_TEXTURE_2D, this->screenColorBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screenColorBuffers[0], 0);


  glGenRenderbuffers(1, &this->screenRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->screenRBO); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->screenRBO);

  GLuint screenAttachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, screenAttachments);


  this->viewport_width = x;
  this->viewport_height = y;

  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->viewport_width/(float)this->viewport_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
}

void unbindTextureUnit(GLenum texture_unit)
{
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::drawModel(Model *model)
{
  for (auto &mesh: model->m_meshes)
  {
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.VBO);


    this->active_shader->setMat4("model", model->getTransform()->getModelMatrix());
    this->active_shader->setMat4("view", this->cam.view);
    this->active_shader->setMat4("projection", this->cam.projection);


    for (int i=0; i<mesh.IBOS.size(); i++)
    {
      mesh.materials[i].diffuseMap.bind( GL_TEXTURE0 );
      mesh.materials[i].specularMap.bind(  GL_TEXTURE1 );
      mesh.materials[i].normalMap.bind(  GL_TEXTURE2 );
      mesh.materials[i].emissionMap.bind(  GL_TEXTURE3 );
    
      this->active_shader->setInt("material.diffuseMap", 0);
      this->active_shader->setInt("material.specularMap", 1);
      this->active_shader->setInt("material.normalMap", 2);
      this->active_shader->setInt("material.emissionMap", 3);
      this->active_shader->setFloat("material.spec_exponent", mesh.materials[i].spec_exponent);


      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]);
      glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0);

      unbindTextureUnit(GL_TEXTURE0);
      unbindTextureUnit(GL_TEXTURE1);
      unbindTextureUnit(GL_TEXTURE2);
      unbindTextureUnit(GL_TEXTURE3);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}

void Renderer::drawLightSource(Model *model, glm::vec3 diffuse_color, int index)
{
  if (this->pointlights_on[index] == false)
    return;

  this->active_shader->setMat4("model", model->getTransform()->getModelMatrix());
  this->active_shader->setMat4("view", this->cam.view);
  this->active_shader->setMat4("projection", this->cam.projection);

  for (auto &mesh: model->m_meshes)
  {
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.VBO);

    for (int i=0; i<mesh.IBOS.size(); i++)
    {
      this->active_shader->setVec3("diffuseColor", diffuse_color);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]);
      glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0);

    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}


Renderer Render::ren;
