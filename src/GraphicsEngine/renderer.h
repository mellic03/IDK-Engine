#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"


#include "camera.h"
#include "lightsource.h"
#include "shader.h"



#define RENDER_DISTANCE 10000.0f
#define NEAR_PLANE_DIST 0.1f



class Renderer {
  
  public:

    Camera cam;

    std::vector<LightSource> lightsources;

    float deltaTime = 0.0f;
    float fov = 90.0f;

    int SCR_width = 1500;
    int SCR_height = 900;

    Renderer();

};