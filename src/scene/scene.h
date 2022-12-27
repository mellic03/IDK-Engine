#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"


class Scene {

  private:
    
    Renderer *ren;

    int _num_active_pointlights;
    int _num_active_spotlights;


  public:

    bool pointlights_on[NUM_POINTLIGHTS];
    bool spotlights_on[NUM_SPOTLIGHTS];

    Player *player;

    // World
    //-------------------------------------------------------
    float gravity = 0.0f;

    PointLight shadowmapped_pointlight;
    PointLight pointlights[NUM_POINTLIGHTS]; std::vector<PointLight> sorted_pointlights;
    SpotLight spotlights[NUM_SPOTLIGHTS];  std::vector<SpotLight> sorted_spotlights;
    //-------------------------------------------------------



    Transform m_temptransform;

    SceneGraph *m_scenegraph;
    NavMesh navmesh;

    Scene(void);

    void setupDepthCubemap(void);
    void updateLights(void);
    void sendLightsToShader(void);

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void useSceneGraph(SceneGraph *scenegraph);
    
    void drawGeometry(SDL_Event *event);    
    void drawLightsources(SDL_Event *event);    

};

namespace World {

  extern Scene scene;

};

