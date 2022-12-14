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
}

GameObject *SceneGraph::objectPtr(int object_id)
{
  for (auto &object: this->m_object_instances)
    if (object.getID() == object_id)
      return &object;

  return nullptr;
}

GameObject *SceneGraph::frontObjectPtr(void)
{
  std::list<GameObject>::iterator ptr = this->m_object_instances.end();
  ptr = std::prev(ptr);

  GameObject *objptr = &*ptr;

  return objptr;
}

void SceneGraph::newObjectInstance(std::string object_name, glm::vec3 pos, glm::vec3 rot)
{
  if (this->objectExists(object_name) == false)
  {
    printf("ERROR: cannot create template of: %s\nObject does not exist within handler.\n", object_name.c_str());
    exit(1);
  }

  int index = this->indexOfObjectName(object_name);

  GameObject newobj = *this->m_object_templates[index];
  *newobj.getPos() = pos;
  *newobj.getRot() = rot;
  newobj.useModel(newobj.m_model);
  newobj.setID(this->m_num_entities);

  this->m_object_instances.push_back(newobj);

  this->m_num_entities += 1;
}


void SceneGraph::deleteObjectInstance(std::string object_name, int instance)
{

}


void SceneGraph::headerInfoToFile(FILE *fh)
{
  fprintf(fh, "#HEADER\n");
  fprintf(fh, "num_entities: %d\n", this->m_num_entities);
  fprintf(fh, "\n");
}

void objectToFile(FILE *fh, GameObject *object)
{
  fprintf(fh, "#START OBJECT\n");

  fprintf(fh, "GameObject: %s\n", object->getName().c_str());
  fprintf(fh, "ID: %d\n", object->getID());
  
  if (object->getTransform()->parent == nullptr)
    fprintf(fh, "parent_ID: %d\n", -1);
  else
    fprintf(fh, "parent_ID: %d\n", object->getParent()->getID());

  fprintf(fh, "ENV: %d, ANIM: %d\n", object->isEnvironmental(), object->isAnimated());
  fprintf(fh, "NPC: %d, HIDDEN: %d\n", object->isNPC(), object->isHidden());


  glm::vec3 pos = *object->getPos();
  glm::vec3 vel = *object->getVel();
  glm::vec3 rot = *object->getRot();
  fprintf(fh, "position: %f %f %f\n", pos.x, pos.y, pos.z);
  fprintf(fh, "velocity: %f %f %f\n", vel.x, vel.y, vel.z);
  fprintf(fh, "rotation: %f %f %f\n", rot.x, rot.y, rot.z);

  fprintf(fh, "#END OBJECT\n");

  fprintf(fh, "\n");
}

void SceneGraph::objectFromFile(FILE *fh)
{
  char buffer[256];
  char stringdata[256];

  GameObject *object;
  glm::vec3 *pos, *vel, *rot;

  while (fgets(buffer, 256, fh) != NULL)
  {
    if (sscanf(buffer, "GameObject: %s", stringdata))
    {
      this->newObjectInstance(std::string(stringdata));
      object = this->frontObjectPtr();
      pos = object->getPos();
      vel = object->getVel();
      rot = object->getRot();
    }

    sscanf(buffer, "position: %f %f %f", &pos->x, &pos->y, &pos->z);
    sscanf(buffer, "velocity: %f %f %f", &vel->x, &vel->y, &vel->z);
    sscanf(buffer, "rotation: %f %f %f", &rot->x, &rot->y, &rot->z);

    if (strcmp(buffer, "#END OBJECT\n") == 0)
      return;
  }

}


void SceneGraph::clearScene(void)
{
  this->m_object_instances.clear();

  this->m_num_entities = 0;    
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
  
  this->headerInfoToFile(fh);

  for (auto &obj: this->m_object_instances)
    objectToFile(fh, &obj);

  fclose(fh);

  return true;
}


bool SceneGraph::importScene(std::string filepath)
{
  this->clearScene();

  FILE *fh = fopen(filepath.c_str(), "r");
  if (fh == NULL)
  {
    printf("Error opening: %s\n", filepath.c_str());
    return false;
  }

  printf("Successfully opened %s\n", filepath.c_str());

  char buffer[256];

  while (fgets(buffer, 256, fh) != NULL)
    if (strcmp(buffer, "#START OBJECT\n") == 0)
      this->objectFromFile(fh);

  rewind(fh);

  // Second pass, apply parent-child relations

  int child_id, parent_id;
  while (fgets(buffer, 256, fh) != NULL)
  {
    if (sscanf(buffer, "ID: %d\n", &child_id));
    if (sscanf(buffer, "parent_ID: %d\n", &parent_id))
    {

      if (parent_id >= 0)
      {
        GameObject *parent = this->objectPtr(parent_id);
        GameObject *child = this->objectPtr(child_id);

        parent->giveChild(child);
      }

      printf("obj: %d, parent: %d\n", child_id, parent_id);
    }

  }



  fclose(fh);

  return true;
}