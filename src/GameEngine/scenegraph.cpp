#include <algorithm>
#include "scenegraph.h"

/** Determine if object/object name already exists in scene
 */
bool SceneGraph::objectExists(std::string object_name)
{
  return std::find(this->m_unique_object_names.begin(), this->m_unique_object_names.end(), object_name) != this->m_unique_object_names.end();
}

int SceneGraph::indexOfObjectName(std::string object_name)
{
  return std::find(this->m_unique_object_names.begin(), this->m_unique_object_names.end(), object_name) - this->m_unique_object_names.begin();
}

void SceneGraph::addObject(GameObject *object)
{
  if (this->objectExists(object->getName()))
    return;

  this->m_unique_object_names.push_back(object->getName());
  this->m_object_templates.push_back(object);
  this->m_object_instances.push_back(std::vector<GameObject>());
}

void SceneGraph::newObjectInstance(std::string object_name, glm::vec3 pos, glm::vec3 rot)
{
  if (this->objectExists(object_name) == false)
  {
    printf("ERROR: cannot create template of: %s\nObject does not exist within handler.\n", object_name.c_str());
    exit(1);
  }

  int index = this->indexOfObjectName(object_name);
  this->m_object_instances[index].push_back(*this->m_object_templates[index]);

  GameObject *object = &this->m_object_instances[index][this->m_object_instances[index].size()-1];
  object->pos = pos;
  object->rot = rot;


  object->addModel(object->m_model);
}

void SceneGraph::deleteObjectInstance(std::string object_name, int instance)
{
  int object_type = this->indexOfObjectName(object_name);
  this->m_object_instances[object_type].erase(this->m_object_instances[object_type].begin() + instance);
}


/** Save object handler data to file in the following format:
 * 
 * object_name  num_instances
 * 
 */
bool SceneGraph::exportScene(std::string filepath)
{

  FILE *fh = fopen(filepath.c_str(), "w");
  if (fh == NULL)
  {
    printf("Error opening: %s\n", filepath.c_str());
    return false;
  }

  int num_object_types = this->m_unique_object_names.size();

  for (int i=0; i<num_object_types; i++)
  {
    
    int num_instances = this->m_object_instances[i].size();
    
    for (int j=0; j<num_instances; j++)
    {
      fprintf(fh, "%s %d %f %f %f %f %f %f\n",
        this->m_unique_object_names[i].c_str(),
        this->m_object_instances[i][j].isHidden(),
        this->m_object_instances[i][j].pos.x,
        this->m_object_instances[i][j].pos.y,
        this->m_object_instances[i][j].pos.z,
        this->m_object_instances[i][j].rot.x,
        this->m_object_instances[i][j].rot.y,
        this->m_object_instances[i][j].rot.z
      );
    }

  }

  fclose(fh);

  return true;
}


bool SceneGraph::importScene(std::string filepath)
{
  FILE *fh = fopen(filepath.c_str(), "r");
  if (fh == NULL)
  {
    printf("Error opening: %s\n", filepath.c_str());
    return false;
  }

  int num_object_types = this->m_unique_object_names.size();


  char object_name[64];
  bool is_hidden;
  glm::vec3 pos = glm::vec3(0.0f);
  glm::vec3 rot = glm::vec3(0.0f);

  int current_object_type = 0;

  char buffer[128];
  while (fgets(buffer, 128, fh) != NULL)
  {
    if ( sscanf(buffer, "%s %d %f %f %f %f %f %f", object_name, &is_hidden,
        &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z) )
    {
      this->newObjectInstance(object_name, pos, rot);
    }
  }

  fclose(fh);

  return true;
}