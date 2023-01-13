#pragma once

#include <list>

#include <fstream>
#include "gameobject/gameobject.h"
#include "player/player.h"
#include "../GraphicsEngine/GraphicsEngine.h"


#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS 2


class SceneGraph {

  private:


  public:

    std::list<Model> m_models;

    std::vector<GameObject> m_object_templates;
    std::list<GameObject> m_object_instances;

    GameObject *pointlight_parent;
    GameObject *spotlight_parent;
    DirLight dirlight;
    PointLight pointlights[MAX_POINTLIGHTS];
    SpotLight spotlights[MAX_SPOTLIGHTS];
    
    std::vector<PointLight *> sorted_active_pointlights, sorted_shadow_pointlights;
    std::vector<PointLight *> sorted_volumetric_pointlights, sorted_volumetric_shadow_pointlights;
  
    std::vector<SpotLight *> sorted_spotlights;
    
    int _num_pointlights = 0,  num_active_pointlights = 0, num_shadow_pointlights = 0;
    int num_volumetric_pointlights = 0, num_volumetric_shadow_pointlights = 0;
    int _num_spotlights  = 0,  _num_active_spotlights  = 0;


    SceneGraph() { };
    
    bool modelExists(std::string model_name);
    int indexOfObjectName(std::string object_name);

    void loadObject(std::string directory);

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
    void objectFromFile_headerData(std::ifstream &stream, std::string &line, Player *player);
    void exportScene(std::string filepath);
    void importScene(std::string filepath, Player *player);
};

