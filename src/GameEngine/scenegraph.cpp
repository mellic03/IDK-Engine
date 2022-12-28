#include <algorithm>
#include "scenegraph.h"


int SceneGraph::indexOfObjectName(std::string object_name)
{
  for (int i=0; i<this->m_object_templates.size(); i++)
    if (this->m_object_templates[i].getTemplateName() == object_name)
      return i;

  return -1;
}


// void SceneGraph::addObject(GameObject *object)
// {
//   if (this->objectExists(object->getName()))
//     return;

//   this->m_unique_object_names.push_back(object->getName());
//   this->m_object_templates.push_back(object);
// }


bool SceneGraph::modelExists(std::string model_name)
{
  for (auto &model: this->m_models)
    if (model.m_name == model_name)
      return true;

  return false;
}


Model *SceneGraph::modelPtr(std::string model_name)
{
  for (auto &model: this->m_models)
    if (model.m_name == model_name)
      return &model;

  return nullptr;
}


GameObject *SceneGraph::objectPtr(int object_id)
{
  for (auto &object: this->m_object_instances)
    if (object.getID() == object_id)
      return &object;

  return nullptr;
}


GameObject *SceneGraph::templatePtr(std::string object_name)
{
  for (auto &object: this->m_object_templates)
    if (object.getTemplateName() == object_name)
      return &object;

  return nullptr;
}


void SceneGraph::loadObject(std::string directory)
{
  std::string filepath = directory + "asset.txt";

  GameObject object;
  Model new_model;

  FILE *fh = fopen(filepath.c_str(), "r");

  char buffer[256];
  char stringdata[256];
  int intdata;
  object.hasGeometry(false);

  while (fgets(buffer, 256, fh) != NULL)
  {
    if (sscanf(buffer, "#gameobject %s", stringdata))
      object.setTemplateName(std::string(stringdata));

    if (sscanf(buffer, "#interactivity %s", stringdata))
      object.setInteractivity(std::string(stringdata));

    if (sscanf(buffer, "#interactivity %s", stringdata))
      object.changePhysState(std::string(stringdata));


    if (sscanf(buffer, "#geometry %s", stringdata))
    {
      new_model.loadDae(directory, std::string(stringdata));

      this->m_models.push_back(new_model);
      object.m_model = &*std::prev(this->m_models.end());
      
      object.hasGeometry(true);
    }

    if (sscanf(buffer, "#collision %s", stringdata))
    {
      object.m_collision_mesh.load(directory + std::string(stringdata));      
      object.hasCollisionMesh(true);
    }

    if (strstr(buffer, "#exit"))
      break;
  }

  this->m_object_templates.push_back(object);

  fclose(fh);
}



GameObject *SceneGraph::rearObjectPtr(void)
{
  std::list<GameObject>::iterator ptr = this->m_object_instances.end();
  ptr = std::prev(ptr);

  GameObject *objptr = &*ptr;

  return objptr;
}

GameObject *SceneGraph::frontObjectPtr(void)
{
  std::list<GameObject>::iterator ptr = this->m_object_instances.begin();

  GameObject *objptr = &*ptr;

  return objptr;
}


void SceneGraph::newObjectInstance(std::string object_name, glm::vec3 pos, glm::vec3 rot)
{
  GameObject *objectptr = this->templatePtr(object_name);

  GameObject newobj = *objectptr;
  *newobj.getPos() = pos;
  // *newobj.getRot() = rot;

  newobj.setName(object_name);
  newobj.setID(this->m_object_instances.size());
  newobj.transform_components.push_back(EntityComponent(COMPONENT_TRANSFORM));

  this->m_object_instances.push_back(newobj);


  this->m_num_entities += 1;
}

