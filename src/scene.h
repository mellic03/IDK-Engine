#pragma once

#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;
    Player *player;


    Model *lightsource_model;


  public:
    std::vector<ModelContainer *> renderContainers;
    std::vector<ModelContainer *> physicsContainers;
    ObjectContainer rendered_objects;

    Scene() { };

    void bindRenderer(Renderer *renptr);
    void bindPlayer(Player *playerptr);

    void addRenderContainer(ModelContainer *render_container);
    void addPhysicsContainer(ModelContainer *physics_container);

    void addLightsourceModel(Model *lightsource_model);
    
    void draw(SDL_Event *event);    

};

