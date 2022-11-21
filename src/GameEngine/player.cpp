

#include "physics.h"
#include "../include/glm/gtx/matrix_interpolation.hpp"

#define G 5.0f

Player::Player()
{
  this->cam = &renderer.cam;

  this->pos = &this->cam->pos;
  this->dir = &this->cam->front;

  this->active_weapon_type = SHOTGUN;
  this->active_weapon = &this->weapons[this->active_weapon_type];
  this->active_weapon->load_model("assets/cube/gun.obj");
  this->active_weapon->hip_pos = glm::vec3(+0.10f, -0.10f, -0.15f);
  this->active_weapon->aim_pos = glm::vec3( 0.00f, -0.015f, -0.10f);
}

SDL_bool mouse_capture = SDL_TRUE;

void Player::key_input(void)
{
  this->vel.y *= 0.999f;
  float friction = 5.0f;
  float damping = 1 / (1 + (renderer.deltaTime * friction));
  this->vel.x *= damping;
  this->vel.z *= damping;
  this->vel.y -= G * renderer.deltaTime;

  this->vel = glm::clamp(this->vel, glm::vec3(-4.5), glm::vec3(4.5));
  *this->pos += this->vel * renderer.deltaTime;

  this->cam->input();

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  glm::vec3 temp_front = { renderer.cam.front.x, 0.0f, renderer.cam.front.z };
  temp_front = glm::normalize(temp_front);

  bool headbob = false;

  if (state[SDL_SCANCODE_W])
  {
    this->vel += this->move_speed * renderer.deltaTime * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_S])
  {
    this->vel -= this->move_speed * renderer.deltaTime * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_A])
  {
    this->vel += this->move_speed * renderer.deltaTime * renderer.cam.right;
    headbob = true;
  }

  if (state[SDL_SCANCODE_D])
  {
    this->vel -= this->move_speed * renderer.deltaTime * renderer.cam.right;
    headbob = true;
  }

  if (state[SDL_SCANCODE_SPACE])
  {
    this->vel.y += this->jump_force * renderer.deltaTime;
  }

}

void Player::mouse_input(SDL_Event *event)
{

  switch (event->type)
  {
    case SDL_MOUSEBUTTONDOWN:
      if (event->button.button == SDL_BUTTON_RIGHT)
      {
        this->active_weapon->aiming = true;
        this->active_weapon->sway /= 8;
        this->move_speed /= 2;
      }
    break;
  
    case SDL_MOUSEBUTTONUP:
      if (event->button.button == SDL_BUTTON_RIGHT)
      {
        this->active_weapon->aiming = false;
        this->active_weapon->sway *= 8;
        this->move_speed *= 2;
      }
    break;
  }

  if (event->type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode())
  {
    this->cam->yaw   += this->cam->rot_speed * renderer.deltaTime * event->motion.xrel;
    this->cam->pitch -= this->cam->rot_speed * renderer.deltaTime * event->motion.yrel;
    this->active_weapon->movement_offset.x -= this->cam->rot_speed * renderer.deltaTime * 0.001f * event->motion.xrel;
    this->active_weapon->movement_offset.y += this->cam->rot_speed * renderer.deltaTime * 0.001f * event->motion.yrel;
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

}


void Player::draw(void)
{
  this->active_weapon->draw();
}