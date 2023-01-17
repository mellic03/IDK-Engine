#pragma once

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../transform.h"


class Camera {

  private:
  
    glm::vec3 m_default_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_default_dir = glm::vec3(0.0f, 0.0f, 0.0f);


  public:

    Transform *m_transform;

    Camera(float near, float far);
    void useTransform(Transform *transform)  { this->m_transform = transform; };

    glm::mat4 modifier_matrix = glm::mat4(1.0f);

    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 *pos = &this->m_default_pos;
    glm::vec3 *pos_worldspace = &this->m_default_pos;
    glm::vec3 target;
    glm::vec3 *dir = &this->m_default_dir;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float headbob_value = 0.0f;

    float *pitch = nullptr, *roll = nullptr, *yaw = nullptr;
    float rot_speed = 0.0003f;

    float fov = 90.0f;

    void init(void);
    void input();

};