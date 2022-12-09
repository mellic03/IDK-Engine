#pragma once

#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;
    Mesh *lightsource_model;


  public:
    Player *player;

    ObjectHandler *object_handler;
    NavMesh navmesh;

    Scene() { };

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void addLightsourceModel(Mesh *lightsource_model);
    
    void addObjectHandler(ObjectHandler *objectHandler) { this->object_handler = objectHandler; };

    void draw(SDL_Event *event);    

};

