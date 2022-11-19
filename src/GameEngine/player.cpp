

#include "physics.h"
#include "../include/glm/gtx/matrix_interpolation.hpp"

#define WEAPON_X +0.10f
#define WEAPON_Y -0.10f
#define WEAPON_Z -0.15f

#define WEAPON_AIM_X +0.00f
#define WEAPON_AIM_Y -0.015f
#define WEAPON_AIM_Z -0.10f

float WEAPON_ANGLE = -0.15f;

Player::Player(GLuint shader)
{
  this->pos = &this->cam.pos;
  this->dir = &this->cam.front;
  this->vel = &this->cam.vel;

  this->weapon.load("assets/cube/gun.obj", shader);
  this->weapon.translate(glm::vec3(WEAPON_X, WEAPON_Y, WEAPON_Z));
  this->weapon.rot_x(WEAPON_ANGLE);
}


void lerp(glm::vec3 *from, glm::vec3 to, float alpha)
{
  glm::vec3 direc = to - *from;
  float dist = glm::length(direc);
  direc *= alpha;
  *from += direc;
}

bool move_to_center = false;

void Player::input(SDL_Event *event, GLuint shader)
{
  *this->vel *= 0.999f;

  this->cam.input(event);

  // this->weapon.rot_y(-this->weapon.rot.y);

  this->cam.projection = glm::perspective(glm::radians(fov), (float)SCR_WDTH / (float)SCR_HGHT, 0.001f, RENDER_DISTANCE);
  glm::mat4 backup_view = this->cam.view;
  this->cam.view = glm::mat4(1.0f);


  this->weapon_x -= (1.0f / this->weapon_sway) * this->weapon_x;
  this->weapon_y -= (1.0f / this->weapon_sway) * this->weapon_y;
  this->weapon_z -= (1.0f / this->weapon_sway) * this->weapon_z;


  float x_offset = 0.001f * sin(this->cam.headbob_value);
  float y_offset = 0.001f * cos(this->cam.headbob_value);

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_W])
  {
    this->weapon_x += x_offset;
    this->weapon_z += 0.001f;
  }

  if (state[SDL_SCANCODE_S])
  {
    this->weapon_x += x_offset;
    this->weapon_z -= 0.001f;
  }

  if (state[SDL_SCANCODE_A])
  {
    this->weapon_x += 0.001f;
    // this->weapon_y += y_offset;
  }

  if (state[SDL_SCANCODE_D])
  {
    this->weapon_x -= 0.001f;
    // this->weapon_y += y_offset;
  }

  while (SDL_PollEvent(event))
  {

    switch (event->type)
    {
      case SDL_MOUSEBUTTONDOWN:
        move_to_center = true;
        this->weapon_sway /= 5;
        this->cam.move_speed /= 2;
      break;
    
      case SDL_MOUSEBUTTONUP:
        move_to_center = false;
        this->weapon_sway *= 5;
        this->cam.move_speed *= 2;
      break;
    }

    if (event->type == SDL_MOUSEMOTION)
    {
      this->cam.yaw   += this->cam.rot_speed * event->motion.xrel;
      this->cam.pitch -= this->cam.rot_speed * event->motion.yrel;
      this->weapon_x -= this->cam.rot_speed * 0.001f * event->motion.xrel;
      this->weapon_y += this->cam.rot_speed * 0.001f * event->motion.yrel;
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

  if (move_to_center)
    lerp(&this->weapon.pos, glm::vec3(WEAPON_AIM_X + this->weapon_x, WEAPON_AIM_Y + this->weapon_y, WEAPON_AIM_Z + this->weapon_z), 0.055f);
  else
    lerp(&this->weapon.pos, glm::vec3(WEAPON_X + this->weapon_x, WEAPON_Y + this->weapon_y, WEAPON_Z + this->weapon_z), 0.055f);
  this->weapon.set_pos(this->weapon.pos);

 
  this->weapon.draw(&this->cam, shader);

  this->cam.view = backup_view;
  this->cam.projection = glm::perspective(glm::radians(fov), (float)SCR_WDTH / (float)SCR_HGHT, NEAR_PLANE_DIST, RENDER_DISTANCE);


}
 