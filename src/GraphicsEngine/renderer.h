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

#define DEFAULT_SCREEN_WIDTH 1500
#define DEFAULT_SCREEN_HEIGHT 900

#define RENDER_DISTANCE 10000.0f
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


  public:
    GLuint FBO;
    GLuint quadVAO, quadVBO, rbo;
    GLuint colorBuffers[2];

    GLuint depthMapFBO, depthMap;
    glm::mat4 lightSpaceMatrix;
    float DIRBIAS = 0.01f;

    GLuint depthCubemap;


    Camera cam;


    float deltaTime = 0.0f;
    float fov = 90.0f;
    float image_kernel[9] = {
      0, 0, 0,
      0, 1, 0,
      0, 0, 0
    };
    float kernel_divisor = 1.0f, kernel_offset_divisor = 600.0f;

    float exposure = 0.5f, gamma = 2.2f;


    int SCR_width = DEFAULT_SCREEN_WIDTH;
    int SCR_height = DEFAULT_SCREEN_HEIGHT;

    glm::vec4 clearColor = {0.0, 0.0, 0.0, 1.0};

    float NM_DIRLIGHTS = 1;
    float NM_POINTLIGHTS = 5;
    float NM_SPOTLIGHTS = 2;

    Shader shaders[10];
    Shader active_shader;
    std::vector<DirLight>    dirlights;
    std::vector<PointLight>  pointlights;
    std::vector<SpotLight>   spotlights;
    glm::vec3 ambient_light = {0.0, 0.0, 0.0};

    float gravity = 5.0f;


    Renderer();

    void bindFrameBufferObject(GLuint buffer, int x, int y);
    void bindTexture(GLuint texture, int x, int y);
    void bindRenderBufferObject(GLuint render_buffer_object, int x, int y);
    void unbindFrameBufferObject(void) { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

    void useShader(ShaderType shader);
    void postProcess(void);
    void useOrthographic(float x, float y, float z);
    void usePerspective(void);

};