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

#include "../glcall.h"

#include "../../transform.h"

#include "../model/model.h"
#include "../camera.h"
#include "../lightsource.h"
#include "../shader.h"
#include "instancedata.h"

#define DEFAULT_SCREEN_WIDTH 1500
#define DEFAULT_SCREEN_HEIGHT 900

#define MAX_BLUR_FBOS 16

#define NUM_DIRLIGHTS 1
#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2


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

    GLuint blurFBOS[MAX_BLUR_FBOS], blurColorBuffers[MAX_BLUR_FBOS];
    int num_blur_FBOs;

    GLuint gbufferFBO, gbufferRBO, gbuffer_position, gbuffer_normal, gbuffer_albedospec, gbuffer_emission;

    GLuint colorFBO, colorRBO, colorBuffer;
    GLuint lightshaftFBO, lightshaftRBO, lightshaftColorBuffer;
    GLuint quadVAO, quadVBO;
    GLuint screenQuadFBO, screenQuadRBO, screenQuadColorBuffers[1];
    GLuint pingPongFBO[2], pingPingRBO[2], pingPongColorBuffers[2];
    GLuint generalFBO, generalRBO, generalColorBuffers[2];
    GLuint billboardFBO, billboardRBO, billboardColorBuffer;

    // Camera/user-facing
    //---------------------------------------------------------------------
    Camera cam = Camera(0.1f, 500.0f);

    float deltaTime = 0.0f;
    float fov = 90.0f, near_plane = 0.1f, far_plane = 500.0f;
    float image_kernel[9] = {
      0, 0, 0,
      0, 1, 0,
      0, 0, 0
    };
    float kernel_divisor = 1.0f, kernel_offset_divisor = 600.0f;

    glm::vec4 clearColor = glm::vec4(29.0f/255.0f, 60.0f/255.0f, 60.0f/255.0f, 1.0f);
    float fog_start = 10, fog_end = 1000;
    float exposure = 1.0f, gamma = 2.2f;

    int viewport_width = DEFAULT_SCREEN_WIDTH;
    int viewport_height = DEFAULT_SCREEN_HEIGHT;
    //---------------------------------------------------------------------


    // Lighting
    //---------------------------------------------------------------------
    VolumetricData volumetrics;
    BloomData bloom;


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

    GLuint depthCubeMapFBOS[2], depthCubeMaps[2];
    GLuint pointlight_depthmapFBO, pointlight_depthCubemap;
    GLuint spotlight_depthmapFBO, spotlight_depthmap;
    //---------------------------------------------------------------------


    float gravity = 0.0f;


    Renderer() { };

    void init(void);


    void compileShaders(void);
    void useShader(ShaderType shader);
    void useShader(Shader *shader);
    void postProcess(void);

    void setupDirLightDepthmap(glm::vec3 dirlightpos, glm::vec3 dirlightdir);
    void perFrameUpdate(void);

    void sendVolumetricData(void);



    void copyTexture(GLuint src, GLuint dest);
    void blurTexture(GLuint input_texture, GLuint output_framebuffer);
    void additiveBlend(GLuint texture_1, GLuint texture_2);

    void genDepthCubemap(GLuint *FBO, GLuint *texture);

    void genBlurBuffers(int x, int y);
    
    void genGBuffer(int x, int y);
    void genGeneralBuffer(int x, int y);
    void genBillboardBuffer(int x, int y);
    void genColorBuffer(int x, int y);
    void genVolLightBuffer(int x, int y);
    void genScreenQuadBuffer(int x, int y);
    void genPingPongBuffer(int x, int y);
    void resize(int x, int y);


    void drawTerrain(Model *model, Transform *transform, float threshold, float epsilon);
    void drawBillboard(Model *model, Transform *transform);
    void drawLightSource(Model *model,Transform *transform, glm::vec3 diffuse);
    void drawModel(Model *model, Transform *transform);
    void drawModelInstanced(Model *model, InstanceData *instance_data);
};


namespace Render {

  extern Renderer ren;

};