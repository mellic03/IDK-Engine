

#include "GameEngine.h"


Player::Player(GLuint shader)
{
  this->pos = &this->cam.pos;
  this->dir = &this->cam.front;

  this->weapon.load("assets/cube/gun.obj", shader);
}

void Player::input(SDL_Event *event, GLuint shader)
{
  this->cam.input(event);
  this->weapon.draw(&this->cam, shader);
}
 