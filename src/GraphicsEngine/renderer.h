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


struct RenderPassTimer {
  int frames_passed = 0;
  clock_t t;

  double color = 0.0;

  double volumetric = 0.0;
  double volumetric_blur = 0.0;


  void beginVolumetric(void)
  {
    this->t = clock();
  };

  void endVolumetric(void)
  {
    if (this->frames_passed >= 60)
    {
      this->frames_passed = 0;
      this->volumetric = 0.0;
    }

    this->volumetric += (clock() - this->t);
    this->frames_passed += 1;
  };

  double getVolumetric(void)
  {
    return (this->volumetric / this->frames_passed) / ((double)t/CLOCKS_PER_SEC);
  };

};


class Renderer {
  
  private:
    float quadVertices[30] = {
      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
      -1.0f, -1.0f,  -0.999f,  0.0f,  0.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,

      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,
       1.0f,  1.0f,  -0.999f,  1.0f,  1.0f
    };

    void createShader(std::string filename, ShaderType type);


  public:

    RenderPassTimer render_pass_timer;

    GLuint gbufferFBO, gbufferRBO, gbuffer_position, gbuffer_normal, gbuffer_albedospec, gbuffer_emission;

    GLuint colorFBO, colorRBO, colorBuffers[2];
    GLuint lightshaftFBO, lightshaftRBO, lightshaftColorBuffer;
    GLuint quadVAO, quadVBO;
    GLuint screenQuadFBO, screenQuadRBO, screenQuadColorBuffers[1];
    GLuint pingPongFBO[2], pingPingRBO[2], pingPongColorBuffers[2];
    GLuint generalFBO, generalRBO, generalColorBuffer;

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
    VolumetricData volumetrics;
    BloomData bloomData;

    float NM_DIRLIGHTS = 1;
    float NM_POINTLIGHTS = 5;
    float NM_SPOTLIGHTS = 2;

    Shader shaders[20];
    Shader *active_shader = &this->shaders[0];
    
    glm::vec3 ambient_light = {0.0, 0.0, 0.0};
    //---------------------------------------------------------------------


    // Shadows
    //---------------------------------------------------------------------
    int DIR_SHADOW_WIDTH = 2048;
    int DIR_SHADOW_HEIGHT = 2048;
    int POINT_SHADOW_WIDTH = 512;
    int POINT_SHADOW_HEIGHT = 512;

    glm::mat4 lightSpaceMatrix;

    GLuint dirlight_depthmapFBO, dirlight_depthmap;
    GLuint pointlight_depthmapFBO, pointlight_depthCubemap;
    GLuint spotlight_depthmapFBO, spotlight_depthmap;
    //---------------------------------------------------------------------


    float gravity = 0.0f;


    Renderer();

    void init(void);

    Shader createShader(std::string filename);

    void compileShaders(void);
    void useShader(ShaderType shader);
    void useShader(Shader *shader);
    void postProcess(void);

    void setupDirLightDepthmap(glm::vec3 dirlightpos, glm::vec3 dirlightdir);
    void setupPointLightDepthCubemap(void);
    void usePerspective(void);

    void update(glm::vec3 pos, glm::vec3 dir);
    void sendLightsToShader(void);
    void sendVolumetricData(void);

    void drawModel(Model *model);
    void drawLightSource(Model *model, glm::vec3 diffuse);


    void copyTexture(GLuint src, GLuint dest);
    void blurTexture(GLuint framebuffer, GLuint texture, int num_passes, float texel_size, float x_strength, float y_strength);
    void additiveBlend(GLuint texture_1, GLuint texture_2);

    void genGBuffer(int x, int y);
    void genGeneralBuffer(int x, int y);
    void genColorBuffer(int x, int y);
    void genVolLightBuffer(int x, int y);
    void genScreenQuadBuffer(int x, int y);
    void genPingPongBuffer(int x, int y);
    void resize(int x, int y);
};


namespace Render {

  extern Renderer ren;

};