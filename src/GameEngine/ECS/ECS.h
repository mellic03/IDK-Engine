#pragma once

#include <vector>

#include "common_ECS.h"
#include "system.h"





class ECS::EntityComponentSystem {

  private:

    // Entities
    //-----------------------------------------------------
    std::vector<GameObject> _entities;
    //-----------------------------------------------------

    // Components
    //-----------------------------------------------------
    std::vector<ECS::SphereCollider> _sphereColliders;
    std::vector<ECS::MeshCollider>   _meshColliders;
    std::vector<ECS::Geometry>       _geometries;
    //-----------------------------------------------------

    // Systems
    //-----------------------------------------------------
    std::vector<std::function<void (void *, void *)>> _systems;
    std::vector<std::function<void (void *, void *)>> _systems_perFrameUpdate;
    //-----------------------------------------------------


  public:
    EntityComponentSystem();
    
    void addEntity();
    void processEntities();


};

