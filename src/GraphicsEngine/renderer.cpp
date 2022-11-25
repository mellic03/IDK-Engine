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

  ShaderSource worldspace_src = parse_shader("assets/shaders/worldspace.vs", "assets/shaders/worldspace.fs");
  Shader worldspace;
  worldspace.set(create_shader(worldspace_src.vertex_source, worldspace_src.fragment_source));
  this->shaders[SHADER_WORLDSPACE] = worldspace;

  ShaderSource viewspace_src = parse_shader("assets/shaders/viewspace.vs", "assets/shaders/viewspace.fs");
  Shader viewspace;
  viewspace.set(create_shader(viewspace_src.vertex_source, viewspace_src.fragment_source));
  this->shaders[SHADER_VIEWSPACE] = viewspace;

  ShaderSource lightsource_src = parse_shader("assets/shaders/lightsource.vs", "assets/shaders/lightsource.fs");
  Shader lightsource;
  lightsource.set(create_shader(lightsource_src.vertex_source, lightsource_src.fragment_source));
  this->shaders[SHADER_LIGHTSOURCE] = lightsource;
  
  ShaderSource renderquad_src = parse_shader("assets/shaders/renderquad.vs", "assets/shaders/renderquad.fs");
  Shader renderquad;
  renderquad.set(create_shader(renderquad_src.vertex_source, renderquad_src.fragment_source));
  this->shaders[SHADER_RENDERQUAD] = renderquad;

  ShaderSource shadowmap_src = parse_shader("assets/shaders/shadowmap.vs", "assets/shaders/shadowmap.fs");
  Shader shadowmap;
  shadowmap.set(create_shader(shadowmap_src.vertex_source, shadowmap_src.fragment_source));
  this->shaders[SHADER_SHADOWMAP] = shadowmap;

  ShaderSource testsrc = parse_shader("assets/shaders/renderquad.vs", "assets/shaders/guassianblur.fs");
  Shader test;
  test.set(create_shader(testsrc.vertex_source, testsrc.fragment_source));
  this->shaders[SHADER_TEST] = test;


  ShaderSource final = parse_shader("assets/shaders/renderquadfinal.vs", "assets/shaders/renderquad.fs");
  Shader fin;
  fin.set(create_shader(final.vertex_source, final.fragment_source));
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


  // Create and setup framebuffer
  //------------------------------------------------------
  glGenFramebuffers(1, &this->FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glGenTextures(2, this->colorBuffers);
  
  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);


  for (GLuint i=0; i<2; i++)
  {
    glBindTexture(GL_TEXTURE_2D, this->colorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->colorBuffers[i], 0);
  }

  glGenRenderbuffers(1, &this->rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, this->rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
  //------------------------------------------------------


  for (int i=0; i<NUM_DIRLIGHTS; i++)
  {
    this->dirlights.push_back(DirLight());

    glGenFramebuffers(1, &this->dirlights[i].depthMapFBO);

    glGenTextures(1, &this->dirlights[i].depthMap);
    glBindTexture(GL_TEXTURE_2D, this->dirlights[i].depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  for (int i=0; i<NUM_POINTLIGHTS; i++)
    this->pointlights.push_back(PointLight());

  this->pointlights[0].diffuse  = {1.0, 1.0, 1.0};
  this->pointlights[0].specular = {0.15, 0.15, 0.15};
  this->pointlights[0].position = {0.0, -5.0, 0.0};

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    this->spotlights.push_back(SpotLight());

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

void Renderer::bindRenderBufferObject(GLuint render_buffer_object, int x, int y)
{
  glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y); 
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

void Renderer::useOrthographic(void)
{
  float near_plane = 1.0f, far_plane = 70.5f;
  glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  glm::mat4 lightView = glm::lookAt(  glm::vec3(-2.0f, 4.0f, -1.0f), 
                                      glm::vec3( 0.0f, 0.0f,  0.0f), 
                                      glm::vec3( 0.0f, 1.0f,  0.0f));

  this->cam.projection = lightProjection;
  this->cam.view = lightView;
  glm::mat4 lightSpaceMatrix = lightProjection * lightView;
  this->shaders[SHADER_SHADOWMAP].use();
  this->shaders[SHADER_SHADOWMAP].setMat4("lightSpaceMatrix", lightSpaceMatrix);

}

void Renderer::usePerspective(void)
{
  this->cam.projection = glm::perspective(glm::radians(this->fov), (float)this->SCR_width/(float)this->SCR_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
  this->cam.view = glm::lookAt(
    this->cam.pos,
    this->cam.pos + this->cam.front,
    this->cam.up
  ); 
}