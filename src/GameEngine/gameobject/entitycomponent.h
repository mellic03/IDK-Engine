#pragma once

#include <vector>
#include <functional>
#include <filesystem>

#include <stdio.h>


#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_sdl.h"
#include "../../include/imgui/imgui_impl_opengl3.h"

#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../physics.h"



enum EntityComponentType {
  COMPONENT_NONE = 0,
  COMPONENT_TRANSFORM,
  COMPONENT_POINT_LIGHT,
  COMPONENT_SPOT_LIGHT,
  COMPONENT_SCRIPT,
  COMPONENT_TERRAIN,
  COMPONENT_LOD,
  COMPONENT_SPHERE_COLLIDER,
  COMPONENT_CAPSULE_COLLIDER,
  COMPONENT_MESH_COLLIDER,
  COMPONENT_NUM_COMPONENTS
};



struct TransformComponent {

};


struct ScriptComponent {
  std::filesystem::path script_path = std::filesystem::path(std::filesystem::current_path() / "LuaScripting/scripts/default.lua");
  std::string script_name = "LuaScripting/scripts/default";
  bool script_changed = false;

  ScriptComponent(std::string line)  { this->script_name = line; };
};


struct TerrainComponent {
  float threshold = 0.5f;
  float epsilon = 0.25f;
};


struct LODComponent {


};


struct PointLightComponent {
  PointLight *pointlight;
};


struct SpotLightComponent {
  SpotLight *spotlight;
};


struct SphereColliderComponent {
  bool visualise = false;

  PhysicsEngine::SphereCollider *sphere_collider = nullptr;
};


struct CapsuleColliderComponent {
  bool visualise = false;

  PhysicsEngine::CapsuleCollider *capsule_collider = nullptr;
};


struct MeshColliderComponent {

};



class EntityComponents {

  private:

    bool _has_component[COMPONENT_NUM_COMPONENTS] = { false };

    TransformComponent            _transform_component;
    std::vector<ScriptComponent>  _script_components;
    TerrainComponent              _terrain_component;
    LODComponent                  _lod_component;
    PointLightComponent           _pointlight_component;
    SpotLightComponent            _spotlight_component;
    SphereColliderComponent       _spherecollider_component;
    CapsuleColliderComponent      _capsulecollider_component;
    MeshColliderComponent         _meshcollider_component;



  public:

    std::vector<ScriptComponent> *getScriptComponents()     { return &this->_script_components; };
    TransformComponent *getTransformComponent()             { return &this->_transform_component; };
    TerrainComponent *getTerrainComponent()                 { return &this->_terrain_component; };
    LODComponent *getLODComponent()                         { return &this->_lod_component; };
    PointLight *getPointLightComponent()                    { return this->_pointlight_component.pointlight; };
    SpotLight *getSpotLightComponent()                      { return this->_spotlight_component.spotlight; };
    SphereColliderComponent *getSphereColliderComponent()   { return &this->_spherecollider_component; };
    CapsuleColliderComponent *getCapsuleColliderComponent() { return &this->_capsulecollider_component; };
    MeshColliderComponent *getMeshColliderComponent()        { return &this->_meshcollider_component; };


    void giveComponent(ScriptComponent    component);
    void giveComponent(TransformComponent component);
    void giveComponent(TerrainComponent   component);
    void giveComponent(LODComponent       component);

    void giveComponent(PointLight *pointlight);
    void giveComponent(SpotLight  *spotlight);

    void giveComponent(PhysicsEngine::SphereCollider  *sphereCollider);
    void giveComponent(PhysicsEngine::CapsuleCollider  *capsuleCollider);
    void giveComponent(MeshColliderComponent meshcollider);


    bool hasComponent(EntityComponentType type);
};




