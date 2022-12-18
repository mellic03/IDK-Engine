#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;
    Mesh *lightsource_model;


  public:
    Player *player;

    SceneGraph *m_scenegraph;
    NavMesh navmesh;

    Scene() { };

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void addLightsourceModel(Mesh *lightsource_model);
    
    void useSceneGraph(SceneGraph *scenegraph) { this->m_scenegraph = scenegraph; };

    void draw(SDL_Event *event);    

};

