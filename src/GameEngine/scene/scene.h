#pragma once

#include "../common.h"
#include "../scenegraph/scenegraph.h"



struct SceneData {

  glm::vec3 _clear_color = glm::vec3(0.0f);

};


namespace Scene {

  extern Player player;
  extern SceneGraph scenegraph;
  static SceneData _scene_data;

  void init();

  void clearColor(glm::vec3 color);

  void sendLightsToShader();
  void sendVolumetricData();
  
  void defaultScene();
  void importScene(std::string filepath);

  void drawDirLightDepthmap();
  void drawPointLightDepthmaps();

  void drawDepthmaps();



  void animationTick();

  void physicsTick_actor_terrain();
  void physicsTick_actor_actor();
  void physicsTick();


  void perFrameUpdate();

  void drawGeometry_batched();
  void drawGeometry_animated();
  void drawGeometry();


  void drawBackground();
  void drawTerrain();
  void drawStatic();
  void drawBillboards();
  void drawBillboardsInstanced();
  void drawActors();
  void drawLightsources();
};

