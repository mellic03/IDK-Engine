#pragma once

#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;

    Mesh *lightsource_model;


  public:
    Player *player;
    std::vector<ModelContainer *> renderContainers;
    std::vector<ModelContainer *> physicsContainers;
    ObjectContainer renderables;
    NavMesh navmesh;

    Scene() { };

    void bindRenderer(Renderer *renptr);
    void bindPlayer(Player *playerptr);

    void addRenderContainer(ModelContainer *render_container);
    void addPhysicsContainer(ModelContainer *physics_container);

    void addLightsourceModel(Mesh *lightsource_model);
    
    void draw(SDL_Event *event);    

};

