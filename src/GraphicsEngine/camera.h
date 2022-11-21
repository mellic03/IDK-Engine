#pragma once

#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif


#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"


class Camera {

  public:
    Camera();

    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 pos;
    glm::vec3 target;
    glm::vec3 dir;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float headbob_value = 0.0f;

    float pitch = 0, roll = 0, yaw = -90;

    #ifdef __unix__
      float rot_speed = 5.0f;
    #elif defined(_WIN32) || defined(WIN32)
      float rot_speed = 55.0f;
    #endif

    void input();

};