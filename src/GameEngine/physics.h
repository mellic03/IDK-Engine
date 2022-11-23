#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"





class ModelContainer {

  public:
    Model *head = NULL;
    ModelContainer(void) {};

    void add(Model *model);
    void draw(Renderer *ren);
    void collide(Player *player);
};

