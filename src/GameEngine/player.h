#pragma once

#include "../include/glm/glm.hpp"
#include "../GraphicsEngine/GraphicsEngine.h"
#include "weapon.h"

enum PlayerState { PSTATE_GROUNDED, PSTATE_FALLING };


class Player {
  
  private:
    PlayerState current_state = PSTATE_FALLING;
    glm::vec3 delta_pos = {0.0f, 0.0f, 0.0f};

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
    float friction = 5.0f;


    Player(Renderer *ren);
    void key_input(Renderer *ren);
    void mouse_input(Renderer *ren, SDL_Event *event);

    void changeState(PlayerState state);
    PlayerState getState(void) { return this->current_state; };
    bool grounded(void) { return this->current_state == PSTATE_GROUNDED; };
    bool ySinceGrounded(void) { return this->delta_pos.y; };

    void draw(Renderer *ren);

};