
#include "camera.h"
#include "renderer.h"

int frame_count = 0;

Camera::Camera()
{
  *this->pos = glm::vec3(0.0f, 0.0f, 3.0f);
  this->target = glm::vec3(0.0f, 0.0f, 0.0f);
  *this->dir = glm::normalize(*this->pos - this->target);
  this->front = glm::normalize(*this->dir);

  glm::vec3 temp_up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->right = glm::normalize(glm::cross(*this->dir, temp_up));

  this->up = glm::cross(this->right, *this->dir);

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

  glm::mat4 rotation = glm::mat4_cast(this->m_transform->orientation);
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), -this->m_transform->position);

  this->view = rotation * translation;

  this->front = glm::inverse(rotation) * glm::vec4(0.0f,  0.0f, -1.0f,  0.0f);
  this->right = glm::inverse(rotation) * glm::vec4(1.0f,  0.0f,  0.0f,  0.0f);

}