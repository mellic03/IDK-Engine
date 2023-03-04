#include "ECS.h"



ECS::EntityComponentSystem::EntityComponentSystem()
{
  this->_systems.resize(static_cast<size_t>(ECS::ComponentType::NUM_COMPONENTS));
  this->_systems[0] = &ECS::System::sphereCollider;
  this->_systems[1] = &ECS::System::meshCollider;
  this->_systems[2] = &ECS::System::geometry;


  this->_systems_perFrameUpdate.resize(static_cast<size_t>(ECS::ComponentType::NUM_COMPONENTS));
  this->_systems_perFrameUpdate[0] = &ECS::System::sphereCollider_perFrameUpdate;
  this->_systems_perFrameUpdate[1] = &ECS::System::meshCollider_perFrameUpdate;
  this->_systems_perFrameUpdate[2] = &ECS::System::geometry_perFrameUpdate;
}


void ECS::EntityComponentSystem::addEntity()
{
}


void ECS::EntityComponentSystem::processEntities()
{
  for (GameObject &object: this->_entities)
  {
    using namespace ECS;
    for (size_t i=0; i<this->_sphereColliders.size(); i++)
    {
      this->_systems[(Uint32)ComponentType::SPHERE_COLLIDER]((void *)&this->_sphereColliders[i], (void *)0);
    }
  }

  // for (auto fn: this->_systems_perFrameUpdate)
  // {
  //   fn(nullptr, nullptr);
  // }

}

