#pragma once

#include "GameEngine/GameEngine.h"
#include "GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;
    Mesh *lightsource_model;


  public:
    Player *player;

    std::vector<GameObject *> m_gameObjects;
    NavMesh navmesh;

    Scene() { };

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);

    void addRenderObject(GameObject *obj) { this->m_gameObjects.push_back(obj); } ;

    void addLightsourceModel(Mesh *lightsource_model);
    
    void draw(SDL_Event *event);    

};

