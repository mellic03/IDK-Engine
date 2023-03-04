#pragma once
#include "component.h"



namespace ECS::System {
  void sphereCollider (void *entity,  void *component);
  void sphereCollider_perFrameUpdate(void *a, void *b);

  void meshCollider   (void *entity,  void *component);
  void meshCollider_perFrameUpdate(void *a, void *b);

  void geometry       (void *entity,  void *component);
  void geometry_perFrameUpdate(void *a, void *b);
};