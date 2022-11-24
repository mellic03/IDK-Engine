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

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

Renderer::Renderer()
{
  glGenFramebuffers(1, &this->FBO);

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
  
  ShaderSource shadowmap_src = parse_shader("assets/shaders/renderquad.vs", "assets/shaders/renderquad.fs");
  Shader shadowmap;
  shadowmap.set(create_shader(shadowmap_src.vertex_source, shadowmap_src.fragment_source));
  this->shaders[SHADER_SHADOWMAP] = shadowmap;



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

  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  
  glGenTextures(1, &this->textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, this->textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureColorbuffer, 0);

  glGenRenderbuffers(1, &this->rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, this->rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);



  for (int i=0; i<NUM_DIRLIGHTS; i++)
  {
    this->dirlights.push_back(DirLight());

    glGenFramebuffers(1, &this->dirlights[i].depthMapFBO);

    glGenTextures(1, &this->dirlights[i].depthMap);
    glBindTexture(GL_TEXTURE_2D, this->dirlights[i].depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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



void Renderer::frameStart(int x, int y)
{
  glViewport(0, 0, x, y);
  glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
  glEnable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);  

}

void Renderer::frameEnd()
{
  this->shaders[SHADER_SHADOWMAP].use();
  this->shaders[SHADER_SHADOWMAP].setInt("screenTexture", 0);
  this->shaders[SHADER_SHADOWMAP].setFloatVector("kernel", 9, this->image_kernel);
  this->shaders[SHADER_SHADOWMAP].setFloat("kernelDivisor", this->kernel_divisor);
  this->shaders[SHADER_SHADOWMAP].setFloat("kernelOffsetDivisor", this->kernel_offset_divisor);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE0, 0);
  glBindVertexArray(this->quadVAO);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glDrawArrays(GL_TRIANGLES, 0, 6); 
}
