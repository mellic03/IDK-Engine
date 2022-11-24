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

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    this->spotlights.push_back(SpotLight());
}



