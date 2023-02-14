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

#include "../transform/transform.h"

#include "../model/model.h"
#include "../camera.h"
#include "../lightsource.h"
#include "../shader.h"
#include "render_debug.h"
#include "primitives/primitive.h"
#include "instancedata.h"
#include "../frustum.h"
#include "renderutil.h"


#define DEFAULT_SCREEN_WIDTH 1500
#define DEFAULT_SCREEN_HEIGHT 900

#define MAX_BLUR_FBOS 16

#define NUM_DIRLIGHTS 1

#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2



class Renderer {
  
  private:
    RenderDebugData _debug_data;
    Primitives _primitives;
    Frustum _frustum;

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

    glm::vec4 clearColor = glm::vec4(59.0f/255.0f, 134.0f/255.0f, 134.0f/255.0f, 1.0f);
    glm::vec4 horizonColor = glm::vec4(161.0f/255.0f, 90.0f/255.0f, 34.0f/255.0f, 1.0f);
    float transition_start = 0.470f;
    float transition_stop = 1.050f;

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
    int DIR_SHADOW_WIDTH = 12;
    int DIR_SHADOW_HEIGHT = 12;
    const int POINT_SHADOW_WIDTH = 512;
    const int POINT_SHADOW_HEIGHT = 512;

    glm::mat4 lightSpaceMatrix;
    CSM cascaded_rsm;


    GLuint depthCubeMapFBOS[2], depthCubeMaps[2];
    GLuint pointlight_depthmapFBO, pointlight_depthCubemap;
    GLuint spotlight_depthmapFBO, spotlight_depthmap;
    //---------------------------------------------------------------------


    float gravity = 0.0f;


    Renderer() { };

    void init(void);
    RenderDebugData *getDebugData()  { return &this->_debug_data; };
    Frustum *getFrustum()            { return &this->_frustum; };

    void compileShaders(void);
    void useShader(ShaderType shader);
    void useShader(Shader *shader);
    void postProcess(void);

    void setupDirLightDepthmap(glm::vec3 dirlightpos, glm::vec3 dirlightdir);
    void perFrameUpdate(int w, int h);

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


    void drawPrimitive(PrimitiveType type, glm::vec3 pos, float radius, Transform *transform);
    void drawPrimitive_box(Transform *transform);
    
    void drawTerrain          (Model *model,  Transform *transform, float threshold, float epsilon);
    void drawBillboard        (Model *model,  Transform *transform);
    void drawLightSource      (Model *model,  Transform *transform, glm::vec3 diffuse);
    void drawModel            (Model *model,  Transform *transform);
    void drawModel_shadowtest            (Model *model,  Transform *transform);
    void drawModelAnimated    (Model *model,  Transform *transform, Animation::AnimationController *animationController);
    void drawModelAnimated_blend(Model *model, Transform *transform, Animation::Animation *anim1, Animation::Animation *anim2, float alpha);

    void drawModelInstanced   (Model *model,  InstanceData *instance_data);

};


namespace Render {

  extern Renderer ren;

};