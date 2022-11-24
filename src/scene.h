#pragma once

#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;
    Player *player;

    std::vector<ModelContainer *> renderContainers;
    std::vector<ModelContainer *> physicsContainers;


  public:
    Scene() { };

    void bindRenderer(Renderer *renptr);
    void bindPlayer(Player *playerptr);
    void addRenderContainer(ModelContainer *render_container);
    void addPhysicsContainer(ModelContainer *physics_container);
    void draw(SDL_Event *event);    
    

};

