#pragma once

#include <list>

#include "gameobject.h"



class SceneGraph {

  private:
    int m_num_entities = 0;

  public:
    std::vector<std::string> m_unique_object_names;

    std::vector<GameObject *> m_object_templates;
    std::list<GameObject> m_object_instances;


    SceneGraph() { };

    bool objectExists(std::string object_name);
    int indexOfObjectName(std::string object_name);
    void addObject(GameObject *object);

    GameObject *objectPtr(int object_id);
    GameObject *frontObjectPtr(void);
    GameObject *rearObjectPtr(void);

    void newObjectInstance(std::string object_name, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f));
    void deleteObjectInstance(std::string object_name, int instance);

    void clearScene(void);
    void headerInfoToFile(FILE *fh);
    bool exportScene(std::string filepath);
    bool importScene(std::string filepath);
    void objectFromFile(FILE *fh);
};

