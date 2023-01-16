#pragma once

#include <vector>
#include <functional>
#include <filesystem>

#include <stdio.h>


#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_sdl.h"
#include "../../include/imgui/imgui_impl_opengl3.h"

#include "../../GraphicsEngine/GraphicsEngine.h"

#include "gameobject.h"
class GameObject;


enum EntityComponentType {
  COMPONENT_NONE = 0,
  COMPONENT_TRANSFORM,
  COMPONENT_LIGHTSOURCE,
  COMPONENT_SCRIPT,
  COMPONENT_TERRAIN,
  COMPONENT_VARIABLE
};

enum DataType {
  DATATYPE_INT,
  DATATYPE_FLOAT,
  DATATYPE_DOUBLE,
  DATATYPE_STRING
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

struct VariableComponent {
  std::string name = "variable";
  DataType data_type;
  int int_data;
  float float_data;
  double double_data;
  std::string string_data;
};

class EntityComponent { 

  private:

    EntityComponentType _component_type = COMPONENT_NONE;
    PointLight *_pointlight = nullptr;
    SpotLight *_spotlight = nullptr;

    void _draw_pointlight(void);
    void _draw_spotlight(void);
    void _draw_lightsource(void);

    void _draw_transform(GameObject *object); 
    void _draw_script(GameObject *object);

    void _draw_terrain(GameObject *object);

    void _draw_variable(GameObject *object);



  public:

    glm::vec3 *diffuse = nullptr;

    // COMPONENT_SCRIPT
    std::filesystem::path script_path;
    std::string script_name = "LuaScripting/scripts/default";
    bool script_changed = false;


    TerrainComponent  terrain_component;
    VariableComponent variable_component;


    EntityComponent(EntityComponentType component_type);
    EntityComponent(EntityComponentType component_type, PointLight *pointlight);
    EntityComponent(EntityComponentType component_type, SpotLight *spotlight);

    void *getComponentData(EntityComponentType component_type);

    void draw(GameObject *object);

    void toFile(std::ofstream &stream);
    void fromFile(std::ifstream &stream);

};



namespace EntityComponentUI {

  bool newComponent(EntityComponentType *component_type);

};