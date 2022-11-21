
#include "camera.h"
#include "renderer.h"

int frame_count = 0;

Camera::Camera()
{
  this->pos = glm::vec3(0.0f, 0.0f, 3.0f);
  this->target = glm::vec3(0.0f, 0.0f, 0.0f);
  this->dir = glm::normalize(this->pos - this->target);
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::vec3 temp_up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->right = glm::normalize(glm::cross(temp_up, this->dir));

  this->up = glm::cross(this->dir, this->right);

  this->view = glm::lookAt(
    this->pos,
    this->pos + this->front,
    this->up
  ); 
  this->pos = glm::vec3(0.0f, 1.0f, 3.0f);

  this->projection = glm::perspective(glm::radians(renderer.fov), (float)SCR_WDTH / (float)SCR_HGHT, 0.1f, RENDER_DISTANCE);
}


void Camera::input()
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  glm::vec3 temp_front = { this->front.x, 0.0f, this->front.z };
  temp_front = glm::normalize(temp_front);

  // this->vel.y = (this->vel.y < -0.1) ? -0.1 : this->vel.y;

  bool headbob = false;


  if (headbob)
    this->headbob_value += 0.035f * renderer.deltaTime;
  
  float y_offset = 0.02f * sin(this->headbob_value);


  if (this->pitch <= -85)
    this->pitch = -85;
  else if (this->pitch >= 85)
    this->pitch = 85;


  this->dir.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  this->dir.y = sin(glm::radians(this->pitch));
  this->dir.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

  this->front = glm::normalize(this->dir);
  this->right = glm::normalize(glm::cross(this->up, this->dir));

  this->view = glm::lookAt(
    this->pos,
    this->pos + this->front,
    this->up
  );

  this->view = glm::translate(this->view, glm::vec3(0.0f, y_offset, 0.0f));

}