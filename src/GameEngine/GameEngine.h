#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"


class GameObject {

  public:
    glm::vec3 pos;
    glm::vec3 vel;


};


class Player {

  public:
  
    Camera cam;
    glm::vec3 *pos;
    glm::vec3 *dir;

    Model weapon;

    Player(GLuint shader);

    void input(SDL_Event *event, GLuint shader);

};