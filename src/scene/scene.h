#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;


  public:

    Player *player;

    // World
    //-------------------------------------------------------
    float gravity = 0.0f;
    std::vector<PointLight> pointlights, sorted_pointlights;
    //-------------------------------------------------------



    Transform m_temptransform;

    SceneGraph *m_scenegraph;
    NavMesh navmesh;

    Scene() { };

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void useSceneGraph(SceneGraph *scenegraph);
    
    void draw(SDL_Event *event);    

};

