
#include "camera.h"
#include "renderer.h"

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

  this->projection;
  this->projection = glm::perspective(glm::radians(90.0f), (float)(SCR_WDTH/SCR_HGHT), 0.1f, RENDER_DISTANCE);
}


void Camera::input(SDL_Event *event)
{

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  glm::vec3 temp_front = { this->front.x, 0.0f, this->front.z };
  temp_front = glm::normalize(temp_front);

  if (state[SDL_SCANCODE_W])
    this->pos += this->move_speed * temp_front;
  else if (state[SDL_SCANCODE_S])
    this->pos -= this->move_speed * temp_front;

  if (state[SDL_SCANCODE_A])
    this->pos += this->move_speed * this->right;
  else if (state[SDL_SCANCODE_D])
    this->pos -= this->move_speed * this->right;

  if (state[SDL_SCANCODE_SPACE])
    this->pos += this->move_speed * this->up;
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


  while (SDL_PollEvent(event))
  {
    if (event->type == SDL_MOUSEMOTION)
    {
      this->yaw   += this->rot_speed * event->motion.xrel;
      this->pitch -= this->rot_speed * event->motion.yrel;
    }

    else if (event->type == SDL_KEYDOWN)
    {
      switch (event->key.keysym.sym)
      {
        case SDLK_ESCAPE:
          exit(0);
          break;
      }
    }
    
    else if (event->type == SDL_QUIT)
      exit(0);
  }
}