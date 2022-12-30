// #include <functional>

#include "player.h"

Player::Player(Renderer *ren)
{
  this->cam = &ren->cam;

  // this->useWeapon(WEAPON_SHOTGUN);
  // this->getWeapon()->loadModel("assets/player/gun/");
  // this->getWeapon()->hip_pos = glm::vec3(+0.10f, -0.10f, -0.15f);
  // this->getWeapon()->aim_pos = glm::vec3( 0.00f, -0.015f, -0.10f);
}

void Player::useGameObject(GameObject *gameobject)
{
  this->m_gameobject = gameobject;
  this->m_gameobject->setName("Player");
  this->m_gameobject->setInteractivity("player");
  
  this->cam->useTransform(this->getTransform());
}


SDL_bool mouse_capture = SDL_TRUE;

void Player::key_input(Renderer *ren)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  this->keylog.clear();
  this->keylog.log(state);


  if (this->fly && this->fly != this->fly_last)
    this->m_gameobject->changePhysState(PHYSICS_NONE);

  else if (!this->fly && this->fly != this->fly_last)
    this->m_gameobject->changePhysState(PHYSICS_FALLING);

  this->fly_last = this->fly;


  switch (this->m_gameobject->getPhysState())
  {
    case (PHYSICS_GROUNDED):
      if (state[SDL_SCANCODE_SPACE])
      {
        this->m_gameobject->changePhysState(PHYSICS_FALLING);
        this->m_gameobject->getVel()->y = 25 * this->jump_force * ren->deltaTime;
      }
      break;
  
    case (PHYSICS_FALLING):
      break;
  }


  glm::vec3 temp_front = { this->cam->front.x, 0.0f, this->cam->front.z };
  temp_front = glm::normalize(temp_front);

  bool headbob = false;

  this->cam->input();

  if (this->fly)
  {
    if (state[SDL_SCANCODE_W])
      *this->getPos() += this->move_speed * ren->deltaTime * temp_front;

    if (state[SDL_SCANCODE_S])
      *this->getPos() -= this->move_speed * ren->deltaTime * temp_front;
   
    if (state[SDL_SCANCODE_A])
      *this->getPos() -= this->move_speed * ren->deltaTime * ren->cam.right;
   
    if (state[SDL_SCANCODE_D])
      *this->getPos() += this->move_speed * ren->deltaTime * ren->cam.right;

    if (state[SDL_SCANCODE_LCTRL])
      this->getPos()->y -= this->move_speed * ren->deltaTime;

    if (state[SDL_SCANCODE_SPACE])
      this->getPos()->y += this->move_speed * ren->deltaTime;
    
    return;
  }
  

  if (state[SDL_SCANCODE_W])
  {
    *this->getVel() += this->move_speed * ren->deltaTime * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_S])
  {
    *this->getVel() -= this->move_speed * ren->deltaTime * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_A])
  {
    *this->getVel() -= this->move_speed * ren->deltaTime * ren->cam.right;
    headbob = true;
  }

  if (state[SDL_SCANCODE_D])
  {
    *this->getVel() += this->move_speed * ren->deltaTime * ren->cam.right;
    headbob = true;
  }

}

void Player::mouse_input(Renderer *ren, SDL_Event *event)
{
  switch (event->type)
  {
    case SDL_MOUSEBUTTONDOWN:
      if (event->button.button == SDL_BUTTON_RIGHT)
      {
        // this->getWeapon()->aiming = true;
        // this->getWeapon()->sway /= 8;
        // this->move_speed /= 2;
      }
    break;
  
    case SDL_MOUSEBUTTONUP:
      if (event->button.button == SDL_BUTTON_RIGHT)
      {
        // this->getWeapon()->aiming = false;
        // this->getWeapon()->sway *= 8;
        // this->move_speed *= 2;
      }
    break;
  }

  if (event->type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode())
  {
    glm::quat dpitch = glm::angleAxis(this->cam->rot_speed * ren->deltaTime * event->motion.yrel, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat dyaw   = glm::angleAxis(this->cam->rot_speed * ren->deltaTime * event->motion.xrel, glm::vec3(0.0f, 1.0f, 0.0f));

    this->getTransform()->orientation = dpitch * this->getTransform()->orientation * dyaw;

    // this->getWeapon()->movement_offset.x -= this->cam->rot_speed * ren->deltaTime * 0.001f * event->motion.xrel;
    // this->getWeapon()->movement_offset.y += this->cam->rot_speed * ren->deltaTime * 0.001f * event->motion.yrel;
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


// void Player::draw(Renderer *ren)
// {
//   this->getWeapon()->draw(ren);
// }