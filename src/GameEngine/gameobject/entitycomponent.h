#pragma once

#include <vector>
#include <functional>
#include <filesystem>

#include <stdio.h>


#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_sdl.h"
#include "../../include/imgui/imgui_impl_opengl3.h"

#include "../../GraphicsEngine/GraphicsEngine.h"



enum EntityComponentType {
  COMPONENT_NONE = 0,
  COMPONENT_TRANSFORM,
  COMPONENT_POINT_LIGHT,
  COMPONENT_SPOT_LIGHT,
  COMPONENT_SCRIPT,
  COMPONENT_TERRAIN,
  COMPONENT_NUM_COMPONENTS
};



struct TerrainComponent {
  float threshold = 0.5f;
  float epsilon   = 0.25f;

  std::vector<Transform> grass_transforms;
  void generateGrassPositions(std::vector<Vertex> &vertices)
  {
    for (auto &vertex: vertices)
    {
      if (glm::normalize(vertex.normal).y > 0.95f)
      {
        for (int i=0; i<10; i++)
        {
          Transform transform;
          *transform.getPos() = vertex.position + glm::vec3(((float)(rand()%4)-2.0f) / (float)(rand()%10), 0.0f, ((float)(rand()%4)-2.0f) / (float)(rand()%10));
          this->grass_transforms.push_back(transform);
        }
      }
    }
  }
};



class EntityComponent { 

  public:
    EntityComponentType component_type = COMPONENT_NONE;
    glm::vec3 *diffuse = nullptr;

    // COMPONENT_POINT_LIGHT
    PointLight *pointlight;
  
    // COMPONENT_SPOT_LIGHT
    SpotLight *spotlight;

    // COMPONENT_SCRIPT
    std::filesystem::path script_path;
    std::string script_name = "LuaScripting/scripts/default";
    bool script_changed = false;

    // COMPONENT_TERRAIN
    float threshold = 0.5f;
    float epsilon = 0.25f;

    EntityComponent() { };
    EntityComponent(EntityComponentType type) { this->component_type = type; };

    void toFile(std::ofstream &stream);
    void fromFile(std::ifstream &stream);

};


struct EntityComponentData {

  private:

    EntityComponent _entity_components[COMPONENT_NUM_COMPONENTS];
    bool _has_entity_component[COMPONENT_NUM_COMPONENTS];

    std::vector<EntityComponent> _script_components;


  public:

    EntityComponentData(void)
    {
      for (int i=0; i<COMPONENT_NUM_COMPONENTS; i++)
      {
        this->_entity_components[i] = EntityComponent((EntityComponentType)i);
        this->_has_entity_component[i] = false;
      }
    };

    bool hasComponent(EntityComponentType type)
    {
      return this->_has_entity_component[type];
    };

    void giveComponent(EntityComponentType type)
    {
      if (type == COMPONENT_SCRIPT)
        this->giveScriptComponent("LuaScripting/scripts/default");
      this->_has_entity_component[type] = true;
    };

    void giveScriptComponent(std::string script_filepath)
    {
      this->_script_components.push_back(EntityComponent(COMPONENT_SCRIPT));
      this->_script_components[this->_script_components.size() - 1].script_name = script_filepath;
      this->_has_entity_component[COMPONENT_SCRIPT] = true;
    };

    void givePointLightComponent(PointLight *pointlight)
    {
      this->_has_entity_component[COMPONENT_POINT_LIGHT] = true;
      this->_entity_components[COMPONENT_POINT_LIGHT].pointlight = pointlight;
    }

    void giveSpotLightComponent(SpotLight *spotlight)
    {
      this->_has_entity_component[COMPONENT_SPOT_LIGHT] = true;
      this->_entity_components[COMPONENT_SPOT_LIGHT].spotlight = spotlight;
    }

    EntityComponent *getComponent(EntityComponentType type)
    {
      if (!this->hasComponent(type))
      {
        printf("Object does not have component: %d\n(entitycomponent.h --> EntityComponentData::getComponent())\n", (int)type);
        exit(1);
      }

      return &this->_entity_components[type];
    };

    std::vector<EntityComponent> *getScriptComponents()
    {
      return &this->_script_components;
    };

};


