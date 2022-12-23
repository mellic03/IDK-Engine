#pragma once

#include "../include/glm/glm.hpp"
#include "gameobject.h"
#include "weapon.h"
#include "../transform.h"

class Player {
  
  private:
  //   WeaponType m_active_weapon = WEAPON_SHOTGUN;
  //   Weapon m_weapons[1];
  //   Weapon *m_active_weapon_ptr;

  public:

    Camera *cam;
    
    GameObject *m_gameobject = nullptr;

    float move_speed = 8.0f;
    float jump_force = 8.0f;
    float friction = 5.0f;

    Player(Renderer *ren);

    // Member Access
    //----------------------------------------------------
    Transform *getTransform(void)       { return this->m_gameobject->getTransform(); };
    glm::vec3 *getPos(void)             { return this->m_gameobject->getPos(); };
    glm::vec3 *getVel(void)             { return this->m_gameobject->getVel(); };
    glm::vec3 *getRot(void)             { return this->m_gameobject->getRot(); };
    //----------------------------------------------------

    void key_input(Renderer *ren);
    void mouse_input(Renderer *ren, SDL_Event *event);


};