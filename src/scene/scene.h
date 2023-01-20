#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"
#include "../include/glm/ext/matrix_transform.hpp"
#include "../include/glm/gtx/euler_angles.hpp"

class Scene {

  private:

    Renderer *ren;

    std::vector<GameObject *> _terrain_queue;
    std::vector<GameObject *> _billboard_queue;
    std::vector<GameObject *> _actor_queue;
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

    void clearColor(glm::vec3 color);

    void sortLights(void);
    void sendLightsToShader(void);

    void useRenderer(Renderer *renptr);
    void usePlayer(Player *playerptr);
    void useSceneGraph(SceneGraph *scenegraph);
    
    void defaultScene(void);
    void importScene(std::string filepath, Player *player);

    void drawDirLightDepthmap(void);
    void drawPointLightDepthmaps(void);

    void drawDepthmaps(void);
    
    void physicsTick_actor_terrain(void);
    void physicsTick_actor_actor(void);
    void physicsTick(void);

    void drawBackground();
    void drawTerrain();
    void drawStatic();

    void drawBillboards();
    void drawBillboardsInstanced();


    void drawActors();
    void drawLightsources();
    void drawGeometry_batched();
    void drawGeometry();

};

namespace World {

  extern Scene scene;

};

