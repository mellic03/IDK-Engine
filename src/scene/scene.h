#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;
    Model *_lightsource_model;


  public:
    Player *player;

    Transform m_temptransform;

    SceneGraph *m_scenegraph;
    NavMesh navmesh;

    Scene() { };

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void useSceneGraph(SceneGraph *scenegraph);
    void addLightsourceModel(Model *lightsource_model) { this->_lightsource_model = lightsource_model; };
    
    void draw(SDL_Event *event);    

};

