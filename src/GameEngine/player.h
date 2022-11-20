#pragma once

#include "../include/glm/glm.hpp"
#include "../GraphicsEngine/GraphicsEngine.h"
#include "weapon.h"



class Player {

  public:

    Camera *cam;
    glm::vec3 *pos;
    glm::vec3 *dir;
    glm::vec3 *vel;

    Weapon weapons[1];
    Weapon *active_weapon;
    WeaponType active_weapon_type = SHOTGUN;

    Model weapon;
    float weapon_sway = 55.0f;
    float weapon_x = 0.0f, weapon_y = 0.0f, weapon_z = 0.0f;

    float height = 0.5f;
    glm::vec3 temp_pos;


    Player();

    void input(SDL_Event *event);

};