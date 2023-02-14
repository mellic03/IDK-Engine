#pragma once

#include <vector>

#include "common_ECS.h"
#include "entity.h"
#include "system.h"





class ECS::EntityComponentSystem {

  private:

    std::vector<ECS::Entity> _entities;

    // Components
    //-----------------------------------------------------
    std::vector<ECS::Geometry>       _geometries;
    std::vector<ECS::SphereCollider> _sphereColliders;
    std::vector<ECS::MeshCollider>   _meshColliders;
    //-----------------------------------------------------

    // Systems
    //-----------------------------------------------------
    std::vector<std::function<void (void *)>> _systemMethods;
    //-----------------------------------------------------


  public:
    EntityComponentSystem() { };
    void processEntities();

};

