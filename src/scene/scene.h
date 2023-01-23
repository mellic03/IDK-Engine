#pragma once

#include "../GameEngine/GameEngine.h"
#include "../GraphicsEngine/GraphicsEngine.h"
#include "../include/glm/ext/matrix_transform.hpp"
#include "../include/glm/gtx/euler_angles.hpp"

class Scene {

  private:

    Renderer *ren;


  public:

    Player *player;

    SceneGraph *m_scenegraph;

    Scene(void);

    void clearColor(glm::vec3 color);

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

    void perFrameUpdate(void);

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



struct SceneData {

  glm::vec3 _clear_color = glm::vec3(0.0f);

};


namespace SceneNamespace {

  extern Player player;
  extern SceneGraph scenegraph;
  static SceneData _scene_data;


  void clearColor(glm::vec3 color);

  void sendLightsToShader();

  void defaultScene();
  void importScene(std::string filepath);

  void drawDirLightDepthmap();
  void drawPointLightDepthmaps();

  void drawDepthmaps();




  void physicsTick_actor_terrain();
  void physicsTick_actor_actor();
  void physicsTick();


  void perFrameUpdate();

  void drawGeometry_batched();
  void drawGeometry();

  void drawBackground();
  void drawTerrain();
  void drawStatic();
  void drawBillboards();
  void drawBillboardsInstanced();
  void drawActors();
  void drawLightsources();
};


namespace World {

  extern Scene scene;

};

