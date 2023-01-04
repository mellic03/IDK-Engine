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

#include "model/model.h"
#include "camera.h"
#include "lightsource.h"
#include "shader.h"

#define DEFAULT_SCREEN_WIDTH 1500
#define DEFAULT_SCREEN_HEIGHT 900

#define RENDER_DISTANCE 500.0f
#define NEAR_PLANE_DIST 0.1f

#define NUM_DIRLIGHTS 1
#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2


class Renderer {
  
  private:
    float quadVertices[24] = {
      -1.0f,  1.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
       1.0f, -1.0f, 1.0f, 0.0f,

      -1.0f,  1.0f, 0.0f, 1.0f,
       1.0f, -1.0f, 1.0f, 0.0f,
       1.0f,  1.0f, 1.0f, 1.0f
    };

    void createShader(std::string filename, ShaderType type);

    glm::mat4 m_active_model_matrix = glm::mat4(1.0f);


  public:
    GLuint colorFBO, colorRBO, colorBuffers[2];
    GLuint lightshaftFBO, lightshaftRBO, lightshaftColorBuffers[1];
    GLuint quadVAO, quadVBO;
    GLuint screenQuadFBO, screenQuadRBO, screenQuadColorBuffers[1];
    GLuint pingPongFBO1, pingPongRBO1, pingPongColorBuffers1[1];
    GLuint pingPongFBO2, pingPongRBO2, pingPongColorBuffers2[1];

    // Camera/user-facing
    //---------------------------------------------------------------------
    Camera cam;

    float deltaTime = 0.0f;
    float fov = 90.0f, far_plane = 2000.0f, near_plane = 1.0f;
    float image_kernel[9] = {
      0, 0, 0,
      0, 1, 0,
      0, 0, 0
    };
    float kernel_divisor = 1.0f, kernel_offset_divisor = 600.0f;

    glm::vec4 clearColor = {0.0, 0.0, 0.0, 1.0};
    float fog_start = 10, fog_end = 1000;
    float exposure = 1.0f, gamma = 2.2f;

    int viewport_width = DEFAULT_SCREEN_WIDTH;
    int viewport_height = DEFAULT_SCREEN_HEIGHT;
    //---------------------------------------------------------------------


    // Lighting
    //---------------------------------------------------------------------
    bool draw_volumetrics = true;
    int volumetric_light_resolution = 1;
    int volumetric_light_blur_passes = 1;

    float NM_DIRLIGHTS = 1;
    float NM_POINTLIGHTS = 5;
    float NM_SPOTLIGHTS = 2;

    Shader shaders[20];
    Shader *active_shader = &this->shaders[0];
    
    glm::vec3 ambient_light = {0.0, 0.0, 0.0};
    //---------------------------------------------------------------------


    // Shadows
    //---------------------------------------------------------------------
    int SHADOW_WIDTH = 512;
    int SHADOW_HEIGHT = 512;

    glm::mat4 lightSpaceMatrix;

    GLuint dirlight_depthmapFBO, dirlight_depthmap;
    GLuint pointlight_depthmapFBO, pointlight_depthCubemap;
    GLuint spotlight_depthmapFBO, spotlight_depthmap;
    //---------------------------------------------------------------------


    float gravity = 0.0f;


    Renderer();

    void init(void);

    void compileShaders(void);
    void useShader(ShaderType shader);
    void postProcess(void);

    void setupDirLightDepthmap(glm::vec3 dirlightpos, glm::vec3 dirlightdir);
    void setupPointLightDepthCubemap(void);
    void usePerspective(void);

    void update(glm::vec3 pos, glm::vec3 dir);
    void sendLightsToShader(void);

    void drawModel(Model *model);
    void drawLightSource(Model *model, glm::vec3 diffuse);

    void renderToQuad(void);

    void genColorBuffer(int x, int y);
    void genVolLightBuffer(int x, int y);
    void genScreenQuadBuffer(int x, int y);
    void genPingPongBuffer(int x, int y);
    void resize(int x, int y);
};


namespace Render {

  extern Renderer ren;

};