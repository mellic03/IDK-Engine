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
  ShaderSource worldspace_src = parse_shader("assets/shaders/basic_worldspace.glsl");
  Shader worldspace;
  worldspace.set(create_shader(worldspace_src.vertex_source, worldspace_src.fragment_source));
  this->shaders[SHADER_WORLDSPACE] = worldspace;

  ShaderSource viewspace_src = parse_shader("assets/shaders/basic_viewspace.glsl");
  Shader viewspace;
  viewspace.set(create_shader(viewspace_src.vertex_source, viewspace_src.fragment_source));
  this->shaders[SHADER_VIEWSPACE] = viewspace;

  ShaderSource lightsource_src = parse_shader("assets/shaders/lightsource.glsl");
  Shader lightsource;
  lightsource.set(create_shader(lightsource_src.vertex_source, lightsource_src.fragment_source));
  this->shaders[SHADER_LIGHTSOURCE] = lightsource;

  for (int i=0; i<8; i++)
    this->createLightSource();  
}



