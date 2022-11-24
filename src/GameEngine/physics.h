#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"





class ModelContainer {

  public:
    std::vector<Model *> models;
    ModelContainer(void) {};

    void add(Model *model);
    void draw(Renderer *ren);
    void collide(Player *player);
    void bindRenderer(Renderer *ren);
    void setShader(Shader *shader);
};

