
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

  this->projection = glm::perspective(glm::radians(fov), (float)SCR_WDTH / (float)SCR_HGHT, 0.1f, RENDER_DISTANCE);
}


void Camera::input(SDL_Event *event)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  glm::vec3 temp_front = { this->front.x, 0.0f, this->front.z };
  temp_front = glm::normalize(temp_front);

  this->pos += this->vel;
  this->vel.y -= 0.0001f;

  bool headbob = false;

  if (state[SDL_SCANCODE_W])
  {
    this->pos += this->move_speed * temp_front;
    headbob = true;
  }
  else if (state[SDL_SCANCODE_S])
  {
    this->pos -= this->move_speed * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_A])
  {
    this->pos += this->move_speed * this->right;
    headbob = true;
  }
  else if (state[SDL_SCANCODE_D])
  {
    this->pos -= this->move_speed * this->right;
    headbob = true;
  }

  if (headbob)
    this->headbob_value += 0.035f;
  
  float y_offset = 0.02f * sin(this->headbob_value);

  if (state[SDL_SCANCODE_SPACE])
  {
    this->pos += this->jump_force * this->up;
    this->vel += this->jump_force * 0.05f * this->up;
  }
  else if (state[SDL_SCANCODE_LCTRL])
    this->pos -= this->move_speed * this->up;


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