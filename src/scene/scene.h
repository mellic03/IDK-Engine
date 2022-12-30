#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:

    Renderer *ren;


    std::vector<GameObject *> _lightsource_queue;


  public:

    Player *player;

    // World
    //-------------------------------------------------------
    float gravity = 0.0f;

    PointLight shadowmapped_pointlight;

    //-------------------------------------------------------

    Transform m_temptransform;

    SceneGraph *m_scenegraph;
    NavMesh navmesh;

    Scene(void);

    void updateLights(void);
    void sendLightsToShader(void);

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void useSceneGraph(SceneGraph *scenegraph);
    
    void clear(void);
    void defaultScene(void);
    void importScene(std::string filepath, Player *player);

    void drawGeometry(SDL_Event *event);    
    void drawLightsources(SDL_Event *event);    

};

namespace World {

  extern Scene scene;

};

