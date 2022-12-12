#pragma once
#include "gameobject.h"



class SceneGraph {

  public:

    std::vector<std::string> m_unique_object_names;

    std::vector<GameObject *> m_object_templates;
    std::vector<std::vector<GameObject>> m_object_instances;


    SceneGraph() { };

    bool objectExists(std::string object_name);
    int indexOfObjectName(std::string object_name);
    void addObject(GameObject *object);

    void newObjectInstance(std::string object_name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f));
    void deleteObjectInstance(std::string object_name, int instance);

    bool exportScene(std::string filepath);
    bool importScene(std::string filepath);

};

