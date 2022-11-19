#pragma once

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"


class Camera {

  private:
  

  public:
    Camera();

    GLuint shader;

    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 pos;
    glm::vec3 vel = {0, 0, 0};
    glm::vec3 target;
    glm::vec3 dir;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float headbob_value = 0.0f;

    float pitch = 0, roll = 0, yaw = -90;
    float rot_speed = 0.05;
    float move_speed = 0.015;
    float jump_force = 0.01;

    void input(SDL_Event *event);

};