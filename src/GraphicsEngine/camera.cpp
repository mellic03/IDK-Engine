
#include "camera.h"
#include "renderer.h"

int frame_count = 0;

Camera::Camera()
{
  *this->pos = glm::vec3(0.0f, 0.0f, 3.0f);
  this->target = glm::vec3(0.0f, 0.0f, 0.0f);
  *this->dir = glm::normalize(*this->pos - this->target);
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::vec3 temp_up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->right = glm::normalize(glm::cross(temp_up, *this->dir));

  this->up = glm::cross(*this->dir, this->right);

  this->view = glm::lookAt(
    *this->pos,
    *this->pos + this->front,
    this->up
  ); 
  *this->pos = glm::vec3(0.0f, 1.0f, 3.0f);

  this->projection = glm::perspective(glm::radians(this->fov), 1.0f, 0.1f, RENDER_DISTANCE);
}

void Camera::init(void)
{


}

void Camera::input()
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (*this->pitch <= -85)
    *this->pitch = -85;
  else if (*this->pitch >= 85)
    *this->pitch = 85;

  this->dir->x = cos(glm::radians(*this->yaw)) * cos(glm::radians(*this->pitch));
  this->dir->y = sin(glm::radians(*this->pitch));
  this->dir->z = sin(glm::radians(*this->yaw)) * cos(glm::radians(*this->pitch));

  glm::vec3 tempdir = this->m_transform->getModelMatrix_noLocalTransform() * glm::vec4(this->dir->x, this->dir->y, this->dir->z, 0.0f);
  glm::vec3 p = this->m_transform->getPos_worldspace();

  this->front = glm::normalize(tempdir);
  this->right = glm::normalize(glm::cross(this->up, tempdir));

  this->view = glm::lookAt(
    p,
    p + this->front,
    this->up
  );

}