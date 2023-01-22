
#include "camera.h"


int frame_count = 0;

Camera::Camera(float near, float far)
{
  *this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
  this->target = glm::vec3(0.0f, 0.0f, 1.0f);
  *this->dir = glm::normalize(this->target - *this->pos);
  this->front = glm::normalize(*this->dir);

  this->up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->right = glm::normalize(glm::cross(*this->dir, this->up));


  this->view = glm::lookAt(
    *this->pos,
    *this->pos + this->front,
    this->up
  ); 
  *this->pos = glm::vec3(0.0f, 1.0f, 3.0f);

  this->projection = glm::perspective(glm::radians(this->fov), 1.0f, near, far);
}

void Camera::init(void)
{

}


void Camera::input()
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  glm::mat4 rotation = glm::inverse(this->m_transform->getOrientation_mat4());
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), -this->m_transform->getPos_worldspace());

  this->view = rotation * translation;

  this->front = glm::inverse(rotation) * glm::vec4(0.0f,  0.0f,  -1.0f,  0.0f);
  this->right = glm::inverse(rotation) * glm::vec4(1.0f,  0.0f,  0.0f,  0.0f);

}