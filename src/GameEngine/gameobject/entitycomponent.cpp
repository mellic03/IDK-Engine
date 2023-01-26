#include "entitycomponent.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../ui/UIEngine.h"










void EntityComponents::giveComponent(ScriptComponent component)
{
  this->_script_components.push_back(component);
};


void EntityComponents::giveComponent(TransformComponent component)
{
  this->_transform_component = component;
  this->_has_component[COMPONENT_TRANSFORM] = true;
};


void EntityComponents::giveComponent(TerrainComponent component)
{
  this->_terrain_component = component;
  this->_has_component[COMPONENT_TERRAIN] = true;
};


void EntityComponents::giveComponent(LODComponent component)
{
  this->_lod_component = component;
  this->_has_component[COMPONENT_LOD] = true;
};



void EntityComponents::giveComponent(PointLight *pointlight)
{
  this->_pointlight_component.pointlight = pointlight;
  this->_has_component[COMPONENT_POINT_LIGHT] = true;
};


void EntityComponents::giveComponent(SpotLight *spotlight)
{
  this->_spotlight_component.spotlight = spotlight;
  this->_has_component[COMPONENT_SPOT_LIGHT] = true;
};


void EntityComponents::giveComponent(PhysicsEngine::SphereCollider *sphereCollider)
{
  this->_spherecollider_component.sphere_collider = sphereCollider;
  this->_has_component[COMPONENT_SPHERE_COLLIDER] = true;
}


void EntityComponents::giveComponent(PhysicsEngine::CapsuleCollider *capsuleCollider)
{
  this->_capsulecollider_component.capsule_collider = capsuleCollider;
  this->_has_component[COMPONENT_CAPSULE_COLLIDER] = true;
}


void EntityComponents::giveComponent(MeshColliderComponent meshcollider)
{
  this->_meshcollider_component = meshcollider;
  this->_has_component[COMPONENT_MESH_COLLIDER] = true;
}



bool EntityComponents::hasComponent(EntityComponentType type) 
{
  if (type == COMPONENT_SCRIPT)
    return this->_script_components.size() > 0;
  else
    return this->_has_component[type];
}
