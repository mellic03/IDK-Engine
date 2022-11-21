#pragma once

#include "../include/glm/glm.hpp"
#include "../GraphicsEngine/GraphicsEngine.h"
#include "weapon.h"



class Player {

  public:

    Camera *cam;
    glm::vec3 *pos;
    glm::vec3 *dir;
    glm::vec3 vel = {0.0f, 0.0f, 0.0f};

    Weapon weapons[1];
    Weapon *active_weapon;
    WeaponType active_weapon_type = SHOTGUN;

    float height = 0.5f;
    glm::vec3 temp_pos;

    float move_speed = 8.0f;
    float jump_force = 8.0f;

    Player(Camera *cam);
    void key_input(Camera *cam);
    void mouse_input(Camera *cam, SDL_Event *event);

    void draw(Camera *cam);

};