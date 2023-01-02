#pragma once

#include <list>

#include <fstream>
#include "gameobject/gameobject.h"
#include "player/player.h"
#include "../GraphicsEngine/GraphicsEngine.h"



class SceneGraph {

  private:


  public:

    std::list<Model> m_models;

    std::vector<GameObject> m_object_templates;
    std::list<GameObject> m_object_instances;

    GameObject *pointlight_parent;
    GameObject *spotlight_parent;
    PointLight pointlights[1]; std::vector<PointLight> sorted_pointlights;
    SpotLight spotlights[2];   std::vector<SpotLight *> sorted_spotlights;
    int _num_pointlights = 0,  _num_active_pointlights = 0;
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
    void updateLights(void);
    void objectFromFile(std::ifstream &stream, std::string &line);
    void objectFromFile_headerData(std::ifstream &stream, std::string &line, Player *player);
    bool exportScene(std::string filepath);
    bool importScene(std::string filepath, Player *player);
};

