

#include "physics.h"
#include "../include/glm/gtx/matrix_interpolation.hpp"


Player::Player()
{
  this->cam = &REN_active_cam;

  this->pos = &this->cam->pos;
  this->dir = &this->cam->front;
  this->vel = &this->cam->vel;

  this->active_weapon_type = SHOTGUN;
  this->active_weapon = &this->weapons[this->active_weapon_type];
  this->active_weapon->load_model("assets/cube/gun.obj");
  this->active_weapon->hip_pos = glm::vec3(+0.10f, -0.10f, -0.15f);
  this->active_weapon->aim_pos = glm::vec3( 0.00f, -0.015f, -0.10f);
}

SDL_bool mouse_capture = SDL_TRUE;

void Player::input(SDL_Event *event)
{
  this->vel->y *= 0.999f;
  float friction = 5.0f;
  float damping = 1 / (1 + (REN_deltaTime * friction));
  this->vel->x *= damping;
  this->vel->z *= damping;

  this->cam->input(event);

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_W])
  {
    // this->weapon_x += x_offset;
    // this->weapon_z += 0.001f;
  }

  if (state[SDL_SCANCODE_S])
  {
    // this->weapon_x += x_offset;
    // this->weapon_z -= 0.001f;
  }

  if (state[SDL_SCANCODE_A])
  {
    // this->weapon_x += 0.001f;
    // this->weapon_y += y_offset;
  }

  if (state[SDL_SCANCODE_D])
  {
    // this->weapon_x -= 0.001f;
    // this->weapon_y += y_offset;
  }

  while (SDL_PollEvent(event))
  {

    switch (event->type)
    {
      case SDL_MOUSEBUTTONDOWN:
        this->active_weapon->aiming = true;
        this->active_weapon->sway /= 8;
        this->cam->move_speed /= 2;
      break;
    
      case SDL_MOUSEBUTTONUP:
        this->active_weapon->aiming = false;
        this->active_weapon->sway *= 8;
        this->cam->move_speed *= 2;
      break;
    }

    if (event->type == SDL_MOUSEMOTION)
    {
      this->cam->yaw   += this->cam->rot_speed * REN_deltaTime * event->motion.xrel;
      this->cam->pitch -= this->cam->rot_speed * REN_deltaTime * event->motion.yrel;
      this->active_weapon->movement_offset.x -= this->cam->rot_speed * REN_deltaTime * 0.001f * event->motion.xrel;
      this->active_weapon->movement_offset.y += this->cam->rot_speed * REN_deltaTime * 0.001f * event->motion.yrel;
    }

    else if (event->type == SDL_KEYDOWN)
    {
      switch (event->key.keysym.sym)
      {
        case SDLK_ESCAPE:
          mouse_capture = (mouse_capture == SDL_TRUE) ? SDL_FALSE : SDL_TRUE;
          SDL_SetRelativeMouseMode(mouse_capture);
          break;
      }
    }

    else if (event->type == SDL_QUIT)
      exit(0);
  }
 
  this->active_weapon->draw();
}
 