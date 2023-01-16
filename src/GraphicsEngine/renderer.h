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

#define NUM_DIRLIGHTS 1
#define NUM_POINTLIGHTS 5
#define NUM_SPOTLIGHTS 2



struct InstanceData {

  GLuint VBO;
  std::vector<glm::mat4>   model_matrices;
  std::vector<Transform *> model_transforms;
  std::vector<Model *>     models;

  void genVBO()
  {
    GLCALL( glDeleteBuffers(1, &this->VBO) );
    GLCALL( glGenBuffers(1, &this->VBO) );
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, this->VBO) );
    GLCALL( glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->model_matrices.size(), &this->model_matrices[0], GL_STATIC_DRAW) );
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
  }


  void addInstance(Model *model, Transform *transform)
  {
    this->model_matrices.push_back(transform->getModelMatrix());
    this->model_transforms.push_back(transform);
    this->models.push_back(model);

    this->genVBO();
  }


  void perFrameUpdate()
  {
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, this->VBO) );

    for (size_t i=0; i<this->model_transforms.size(); i++)
    {
      this->model_matrices[i] = this->model_transforms[i]->getModelMatrix();
    }
  
    for (auto &model: this->models)
    {
      for (auto &mesh: model->m_meshes)
      {
        GLCALL( glBindVertexArray(mesh.VAO) );

        int pos = 5;
        int pos1 = pos + 0;
        int pos2 = pos + 1;
        int pos3 = pos + 2;
        int pos4 = pos + 3;

        GLCALL( glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(0)) );
        GLCALL( glEnableVertexAttribArray(pos1) );

        GLCALL( glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1*sizeof(glm::vec4))) );
        GLCALL( glEnableVertexAttribArray(pos2) );

        GLCALL( glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2*sizeof(glm::vec4))) );
        GLCALL( glEnableVertexAttribArray(pos3) );

        GLCALL( glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3*sizeof(glm::vec4))) );
        GLCALL( glEnableVertexAttribArray(pos4) );

        GLCALL( glVertexAttribDivisor(pos1, 1) );
        GLCALL( glVertexAttribDivisor(pos2, 1) );
        GLCALL( glVertexAttribDivisor(pos3, 1) );
        GLCALL( glVertexAttribDivisor(pos4, 1) );

        GLCALL( glBindVertexArray(0) );
      }
    }
  }
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
    GLuint gbufferFBO, gbufferRBO, gbuffer_position, gbuffer_normal, gbuffer_albedospec, gbuffer_emission;

    GLuint colorFBO, colorRBO, colorBuffers[2];
    GLuint lightshaftFBO, lightshaftRBO, lightshaftColorBuffer;
    GLuint quadVAO, quadVBO;
    GLuint screenQuadFBO, screenQuadRBO, screenQuadColorBuffers[1];
    GLuint pingPongFBO[2], pingPingRBO[2], pingPongColorBuffers[2];
    GLuint generalFBO, generalRBO, generalColorBuffer;
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

    void drawTerrain(Model *model, Transform *transform, float threshold, float epsilon);
    void drawBillboard(Model *model, Transform *transform);
    void drawLightSource(Model *model,Transform *transform, glm::vec3 diffuse);
    void drawModel(Model *model, Transform *transform);


    void copyTexture(GLuint src, GLuint dest);
    void blurTexture(GLuint framebuffer, GLuint texture, int num_passes, float texel_size, float x_strength, float y_strength);
    void additiveBlend(GLuint texture_1, GLuint texture_2);

    void genDepthCubemap(GLuint *FBO, GLuint *texture);

    void genGBuffer(int x, int y);
    void genGeneralBuffer(int x, int y);
    void genBillboardBuffer(int x, int y);
    void genColorBuffer(int x, int y);
    void genVolLightBuffer(int x, int y);
    void genScreenQuadBuffer(int x, int y);
    void genPingPongBuffer(int x, int y);
    void resize(int x, int y);
};


namespace Render {

  extern Renderer ren;

};