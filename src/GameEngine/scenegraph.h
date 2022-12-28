#pragma once

#include <list>

#include "gameobject/gameobject.h"



class SceneGraph {

  private:
    int _num_models = 0;
    int m_num_entities = 0;


  public:

    int m_selected_instance = 0;

    std::list<Model> m_models;

    std::vector<GameObject> m_object_templates;
    std::list<GameObject> m_object_instances;
    std::vector<GameObject *> m_lightsource_instances;
    std::vector<GameObject *> m_pointlight_instances;
    std::vector<GameObject *> m_spotlight_instances;


    SceneGraph() { };
    
    bool modelExists(std::string model_name);
    int indexOfObjectName(std::string object_name);

    void loadObject(std::string directory);


    Model *modelPtr(std::string model_name);
    GameObject *objectPtr(int object_id);
    GameObject *templatePtr(std::string object_name);
    GameObject *frontObjectPtr(void);
    GameObject *rearObjectPtr(void);

    void newObjectInstance(std::string object_name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f));
    // void deleteObjectInstance(std::string object_name, int instance);

    // void clearScene(void);
    // void headerInfoToFile(FILE *fh);
    // bool exportScene(std::string filepath);
    // bool importScene(std::string filepath);
    // void objectFromFile(FILE *fh);
};

