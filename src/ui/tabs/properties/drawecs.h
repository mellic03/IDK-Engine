#pragma once

#include "../../UIEngine.h"
#include "../../../GameEngine/GameEngine.h"



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

};