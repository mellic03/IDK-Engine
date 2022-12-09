#include "gameobject.h"


// A std::vector<std::string> stores the unique name of each object type


class ObjectHandler {

  public:

    std::vector<std::string> m_unique_object_names;

    std::vector<GameObject *> m_object_templates;
    std::vector<std::vector<GameObject>> m_object_instances;


    ObjectHandler() { };

    bool objectExists(std::string object_name);
    int indexOfObjectName(std::string object_name);
    void addObject(GameObject *object);

    void newObjectInstance(std::string object_name);

    bool exportHandler(std::string filepath);
    bool importHandler(std::string filepath);

};

