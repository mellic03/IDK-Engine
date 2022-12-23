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
  *newobj.getRot() = rot;

  newobj.setName(object_name);
  newobj.setID(this->m_object_instances.size());
  this->m_object_instances.push_back(newobj);

  this->m_num_entities += 1;
}


// void SceneGraph::deleteObjectInstance(std::string object_name, int instance)
// {

// }


// void SceneGraph::headerInfoToFile(FILE *fh)
// {
//   fprintf(fh, "#HEADER\n");
//   fprintf(fh, "num_entities: %d\n", this->m_num_entities);
//   fprintf(fh, "\n");
// }

// void objectToFile(FILE *fh, GameObject *object)
// {
//   fprintf(fh, "#START OBJECT\n");

//   fprintf(fh, "GameObject: %s\n", object->getName().c_str());
//   fprintf(fh, "ID: %d\n", object->getID());
  
//   if (object->getTransform()->parent == nullptr)
//     fprintf(fh, "parent_ID: %d\n", -1);
//   else
//     fprintf(fh, "parent_ID: %d\n", object->getParent()->getID());

//   fprintf(fh, "given_name:\n%s\n", object->getGivenName().c_str());
    
//   glm::vec3 pos = *object->getPos();
//   glm::vec3 vel = *object->getVel();
//   glm::vec3 rot = *object->getRot();
//   fprintf(fh, "position: %f %f %f\n", pos.x, pos.y, pos.z);
//   fprintf(fh, "velocity: %f %f %f\n", vel.x, vel.y, vel.z);
//   fprintf(fh, "rotation: %f %f %f\n", rot.x, rot.y, rot.z);

//   fprintf(fh, "#END OBJECT\n");

//   fprintf(fh, "\n");
// }

// void SceneGraph::objectFromFile(FILE *fh)
// {
//   char buffer[256];
//   char stringdata[256];

//   GameObject *object;
//   glm::vec3 *pos, *vel, *rot;

//   while (fgets(buffer, 256, fh) != NULL)
//   {
//     if (sscanf(buffer, "GameObject: %s", stringdata))
//     {
//       this->newObjectInstance(std::string(stringdata));
//       object = this->frontObjectPtr();
//       pos = object->getPos();
//       vel = object->getVel();
//       rot = object->getRot();
//     }

//     if (sscanf(buffer, "given_name: %s", stringdata))
//     {
//       fgets(buffer, 256, fh);
//       object->setGivenName(std::string(buffer));
//     }

//     sscanf(buffer, "position: %f %f %f", &pos->x, &pos->y, &pos->z);
//     sscanf(buffer, "velocity: %f %f %f", &vel->x, &vel->y, &vel->z);
//     sscanf(buffer, "rotation: %f %f %f", &rot->x, &rot->y, &rot->z);

//     if (strcmp(buffer, "#END OBJECT\n") == 0)
//       return;
//   }

// }


// void SceneGraph::clearScene(void)
// {
//   this->m_object_instances.clear();

//   this->m_num_entities = 0;    
// }

// /** Save object handler data to file in the following format:
//  * 
//  * object_name  num_instances
//  * 
//  */
// bool SceneGraph::exportScene(std::string filepath)
// {

//   FILE *fh = fopen(filepath.c_str(), "w");
//   if (fh == NULL)
//   {
//     printf("Error opening: %s\n", filepath.c_str());
//     return false;
//   }
  
//   this->headerInfoToFile(fh);

//   for (auto &obj: this->m_object_instances)
//     objectToFile(fh, &obj);

//   fclose(fh);

//   return true;
// }


// bool SceneGraph::importScene(std::string filepath)
// {
//   this->clearScene();

//   FILE *fh = fopen(filepath.c_str(), "r");
//   if (fh == NULL)
//   {
//     printf("Error opening: %s\n", filepath.c_str());
//     return false;
//   }

//   printf("Successfully opened %s\n", filepath.c_str());

//   char buffer[256];

//   while (fgets(buffer, 256, fh) != NULL)
//     if (strcmp(buffer, "#START OBJECT\n") == 0)
//       this->objectFromFile(fh);

//   rewind(fh);

//   // Second pass, apply parent-child relations
//   bool first_object = true;
//   int child_id, parent_id;
//   while (fgets(buffer, 256, fh) != NULL)
//   {
//     if (sscanf(buffer, "ID: %d\n", &child_id));
//     if (sscanf(buffer, "parent_ID: %d\n", &parent_id))
//     {
//       if (parent_id >= 0)
//       {
//         GameObject *parent = this->objectPtr(parent_id);
//         GameObject *child = this->objectPtr(child_id);

//         parent->giveChild(child);
//       }
//     }

//   }



//   fclose(fh);

//   return true;
// }