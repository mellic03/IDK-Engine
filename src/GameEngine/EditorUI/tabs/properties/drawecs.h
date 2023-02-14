#pragma once

#include "../../UIEngine.h"
#include "../../common_EditorUI.h"



namespace DrawECS {

  void pointlight(GameObject *object, PointLight *pointlight);
  void transform(GameObject *object, TransformComponent *transformComponent);
  void script(GameObject *object, ScriptComponent *scriptComponent);
  void terrain(GameObject *object, TerrainComponent *terrain_component);
  void lod(GameObject *object, LODComponent *lod_component);
  void sphere(GameObject *object, SphereColliderComponent *sphereColliderComponent);
  void capsule(GameObject *object, CapsuleColliderComponent *capsuleColliderComponent);
  void animation(GameObject *object);
  void physics(GameObject *object);
  void navigation(GameObject *object);
  void boundingbox(GameObject *object);

};