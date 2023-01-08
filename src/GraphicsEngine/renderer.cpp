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
#include "../scene/scene.h"

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

void Renderer::compileShaders(void)
{
  this->createShader("terrain",        SHADER_TERRAIN);
  this->createShader("worldspace",     SHADER_WORLDSPACE);
  this->createShader("weapon",         SHADER_WEAPON);
  this->createShader("lightsource",    SHADER_LIGHTSOURCE);
  this->createShader("screenquad",     SHADER_SCREENQUAD);

  this->createShader("normals",           SHADER_NORMALS);
  this->createShader("volumetriclights",  SHADER_VOLUMETRIC_LIGHT);
  this->createShader("blur",              SHADER_BLUR);

  // Shadows 
  //------------------------------------------------------
  this->createShader("dirshadow",         SHADER_DIRSHADOW);
  this->createShader("gbuffer",                SHADER_GBUFFER);


  ShaderSource pointshadow_src = parse_shader("assets/shaders/pointshadow.vs", "assets/shaders/pointshadow.fs", "assets/shaders/pointshadow.gs");
  Shader pointshadow;
  pointshadow.set(create_shader(pointshadow_src.vertex_source, pointshadow_src.fragment_source, pointshadow_src.geometry_source));
  this->shaders[SHADER_POINTSHADOW] = pointshadow;
  //------------------------------------------------------
}

void Renderer::init(void)
{

  this->compileShaders();

  // Generate screen quad
  //------------------------------------------------------
  glGenVertexArrays(1, &this->quadVAO);
  glGenBuffers(1, &this->quadVBO);
  glBindVertexArray(this->quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(this->quadVertices), &this->quadVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  //------------------------------------------------------


  // Create colour framebuffer
  //------------------------------------------------------
  this->genColorBuffer(2560, 2560);
  this->genVolLightBuffer(2560, 2560);
  this->genScreenQuadBuffer(2560, 2560);

  //------------------------------------------------------


  // Directional light depthmap
  //------------------------------------------------------
  glGenFramebuffers(1, &this->dirlight_depthmapFBO); 

  glGenTextures(1, &this->dirlight_depthmap);
  glGenTextures(1, &this->dirlight_depthmap);
  glBindTexture(GL_TEXTURE_2D, this->dirlight_depthmap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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


  // Pointlight depth cubemap
  //------------------------------------------------------
  glGenFramebuffers(1, &this->pointlight_depthmapFBO); 

  glGenTextures(1, &this->pointlight_depthCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->pointlight_depthCubemap);
  for (unsigned int i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, this->pointlight_depthmapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->pointlight_depthCubemap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //------------------------------------------------------
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


void Renderer::setupDirLightDepthmap(glm::vec3 dirlightpos, glm::vec3 dirlightdir)
{
  float aspect = (float)this->DIR_SHADOW_WIDTH / (float)this->DIR_SHADOW_HEIGHT;
  float near = 1.0f;
  float far = 25.0f;
  glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
  
  glm::mat4 lightView = glm::lookAt( this->cam.m_transform->getPos_worldspace() + dirlightpos, 
                                     this->cam.m_transform->getPos_worldspace(),
                                     glm::vec3( 0.0f, 1.0f,  0.0f));

  this->lightSpaceMatrix = lightProjection * lightView;
  this->active_shader->setMat4("lightSpaceMatrix", this->lightSpaceMatrix);
}


void Renderer::setupPointLightDepthCubemap(void)
{
  float aspect = (float)this->POINT_SHADOW_WIDTH / (float)this->POINT_SHADOW_HEIGHT;
  float near = 1.0f;
  float far = 25.0f;
  glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

  std::vector<glm::mat4> shadowTransforms;
  PointLight *pointlight = &World::scene.m_scenegraph->pointlights[0];
  glm::vec3 lightPos = pointlight->m_transform->getPos_worldspace();
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

}


void Renderer::sendLightsToShader(void)
{

}


void Renderer::sendVolumetricData(void)
{
  this->active_shader->setInt("volumetrics.num_samples",         this->volumetrics.num_samples);
  this->active_shader->setFloat("volumetrics.step_size",         this->volumetrics.step_size);
  this->active_shader->setFloat("volumetrics.step_multiplier",   this->volumetrics.step_multiplier);
  this->active_shader->setInt("volumetrics.resolution_divisor",  this->volumetrics.resolution_divisor);
  this->active_shader->setInt("volumetrics.num_blur_passes",     this->volumetrics.num_blur_passes);
}


void Renderer::genGBuffer(int x, int y)
{
  glDeleteTextures(1, &this->gbuffer_position);
  glDeleteTextures(1, &this->gbuffer_normal);
  glDeleteTextures(1, &this->gbuffer_albedospec);

  glDeleteRenderbuffers(1, &this->gbufferRBO);
  glDeleteFramebuffers(1, &this->gbufferFBO);

  glGenFramebuffers(1, &this->gbufferFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->gbufferFBO);
    
  // - position color buffer
  glGenTextures(1, &this->gbuffer_position);
  glBindTexture(GL_TEXTURE_2D, this->gbuffer_position);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gbuffer_position, 0);
    
  // - normal color buffer
  glGenTextures(1, &this->gbuffer_normal);
  glBindTexture(GL_TEXTURE_2D, this->gbuffer_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gbuffer_normal, 0);

  // - tangent color buffer
  glGenTextures(1, &this->gbuffer_tangent);
  glBindTexture(GL_TEXTURE_2D, this->gbuffer_tangent);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->gbuffer_tangent, 0);
    
  // - color + specular color buffer
  glGenTextures(1, &this->gbuffer_albedospec);
  glBindTexture(GL_TEXTURE_2D, this->gbuffer_albedospec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->gbuffer_albedospec, 0);
    
  // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
  unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
  glDrawBuffers(4, attachments);


  glGenRenderbuffers(1, &this->gbufferRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->gbufferRBO); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->gbufferRBO);


  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genColorBuffer(int x, int y)
{
  glDeleteTextures(1, &this->colorBuffers[0]);
  glDeleteRenderbuffers(1, &this->colorRBO);
  glDeleteFramebuffers(1, &this->colorFBO);

  glGenFramebuffers(1, &this->colorFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->colorFBO);
  glGenTextures(1, &this->colorBuffers[0]);
  
  glBindTexture(GL_TEXTURE_2D, this->colorBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorBuffers[0], 0);


  glGenRenderbuffers(1, &this->colorRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, this->colorRBO); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->colorRBO);


  GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::genVolLightBuffer(int x, int y)
{
  glDeleteTextures(1, &this->lightshaftColorBuffers[0]);
  glDeleteRenderbuffers(1, &this->colorRBO);
  glDeleteFramebuffers(1, &this->lightshaftFBO);

  glGenFramebuffers(1, &this->lightshaftFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->lightshaftFBO);
  glGenTextures(1, &this->lightshaftColorBuffers[0]);

  glBindTexture(GL_TEXTURE_2D, this->lightshaftColorBuffers[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, this->lightshaftColorBuffers[0], 0);


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
  this->genColorBuffer(x, y);
  this->genVolLightBuffer(x/this->volumetrics.resolution_divisor, y/this->volumetrics.resolution_divisor);
  this->genPingPongBuffer(x, y);
  this->genScreenQuadBuffer(x, y);


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
  this->active_shader->setMat4("model", model->getTransform()->getModelMatrix());
  this->active_shader->setMat4("view", this->cam.view);
  this->active_shader->setMat4("projection", this->cam.projection);

  for (auto &mesh: model->m_meshes)
  {
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.VBO);

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


void Renderer::drawLightSource(Model *model, glm::vec3 diffuse)
{
  this->active_shader->setMat4("model", model->getTransform()->getModelMatrix());
  this->active_shader->setMat4("view", this->cam.view);
  this->active_shader->setMat4("projection", this->cam.projection);

  for (auto &mesh: model->m_meshes)
  {
    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.VBO);

    for (int i=0; i<mesh.IBOS.size(); i++)
    {
      this->active_shader->setVec3("diffuseColor", diffuse);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBOS[i]);
      glDrawElements(GL_TRIANGLES, mesh.indices[i].size(), GL_UNSIGNED_INT, (void *)0);

    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
}


Renderer Render::ren;
