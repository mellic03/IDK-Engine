#pragma once

#include <list>
#include <map>

#include <fstream>
#include "../gameobject/gameobject.h"
#include "../gameobject/gameobjectutil.h"
#include "../player/player.h"
#include "../../GraphicsEngine/GraphicsEngine.h"


#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS 2


class SceneGraph {

  private:
    std::list<GameObject *>  _object_templates_by_type[GAMEOBJECT_NUM_TYPES];
    std::list<GameObject *>  _object_instances_by_type[GAMEOBJECT_NUM_TYPES];
    std::list<GameObject *>  _object_instances_by_type_instanced[GAMEOBJECT_NUM_TYPES];

    std::map<std::string, InstanceData> _instance_data;


  public:

    std::list<Model> m_models;

    std::list<GameObject>     m_object_templates;
    std::list<GameObject>     m_object_instances;
  
    std::list<GameObject *>   m_dirlight_instances;
    std::list<GameObject *>   m_spotlight_instances;
    std::list<GameObject *>   m_pointlight_instances;

    std::list<GameObject *>   m_selectable_instances;
    GameObject *player_object;


    GameObject *pointlight_parent;
    GameObject *spotlight_parent;
    DirLight dirlight;
    PointLight pointlights[MAX_POINTLIGHTS];
    SpotLight spotlights[MAX_SPOTLIGHTS];
    
    PointLight *sorted_active_pointlights[MAX_POINTLIGHTS], *sorted_shadow_pointlights[MAX_POINTLIGHTS];
    PointLight *sorted_volumetric_pointlights[MAX_POINTLIGHTS], *sorted_volumetric_shadow_pointlights[MAX_POINTLIGHTS];
  
    std::vector<SpotLight *> sorted_spotlights;
    
    int _num_pointlights = 0,  num_active_pointlights = 0, num_shadow_pointlights = 0;
    int num_volumetric_pointlights = 0, num_volumetric_shadow_pointlights = 0;
    int _num_spotlights  = 0,  _num_active_spotlights  = 0;


    SceneGraph();
    
    void directoryloadrecursething(std::filesystem::path filepath);
    void loadObject(std::string directory);
    void loadObjects(std::string index_filepath);

    void addInstanceData(std::string template_name, Model *model, Transform *transform);
    std::map<std::string, InstanceData> *getInstanceData(void);
    void perFrameUpdate(void);

    Model *modelPtr(std::string model_name);
    GameObject *objectPtr(int object_id);
    GameObject *objectPtr(std::string object_name);
    GameObject *templatePtr(std::string object_name);
    GameObject *frontObjectPtr(void);
    GameObject *rearObjectPtr(void);

    void newObjectInstance(std::string object_name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f));

    void clearScene(void);
    void defaultScene(void);
    void sortLights(void);
    void objectFromFile(std::ifstream &stream, std::string &line);
    void objectFromFile(std::ifstream &stream, std::string &line, Player *player);
    void exportScene(std::string filepath);
    void importScene(std::string filepath, Player *player);

    std::list<GameObject *> *getTemplatesByType(GameObjectType object_type);
    std::list<GameObject *> *getInstancesByType(GameObjectType object_type);
    std::list<GameObject *> *getInstancesByType(GameObjectType object_type, InstancingType instancing);

};

