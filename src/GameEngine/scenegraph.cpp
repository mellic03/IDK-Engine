#include <algorithm>
#include "scenegraph.h"


int SceneGraph::indexOfObjectName(std::string object_name)
{
  for (int i=0; i<this->m_object_templates.size(); i++)
    if (this->m_object_templates[i].getTemplateName() == object_name)
      return i;

  return -1;
}


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
  Model vlight_model;

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

    if (sscanf(buffer, "#physics %s", stringdata))
      object.changePhysState(std::string(stringdata));


    if (sscanf(buffer, "#geometry %s", stringdata))
    {
      new_model.loadDae(directory, std::string(stringdata));
      this->m_models.push_back(new_model);
      object.m_model = &*std::prev(this->m_models.end());
      object.hasGeometry(true);
    }

    if (sscanf(buffer, "#volumetriclight %s", stringdata))
    {
      vlight_model.loadDae(directory, std::string(stringdata));
      this->m_models.push_back(vlight_model);
      object.m_volumetric_light_model = &*std::prev(this->m_models.end());
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
  if (object_name == "pointlight" && this->_num_pointlights >= 1)
    return;

  if (object_name == "spotlight" && this->_num_spotlights >= 2)
    return;


  GameObject *objectptr = this->templatePtr(object_name);
  if (objectptr == nullptr)
  {
    printf("Null object pointer (SceneGraph::newObjectInstance)\n");
    exit(1);
  }

  GameObject newobj = *objectptr;
  newobj.m_parent = nullptr;
  *newobj.getPos() = pos;


  newobj.setName(object_name);
  newobj.setID(this->m_object_instances.size());

  newobj.transform_components.push_back(EntityComponent(COMPONENT_TRANSFORM));


  this->m_object_instances.push_back(newobj);

  if (object_name == "pointlight")
  {
    this->rearObjectPtr()->hasGeometry(false);
    this->rearObjectPtr()->lightsource_components.push_back(EntityComponent(COMPONENT_LIGHTSOURCE, &this->pointlights[this->_num_pointlights]));
    this->pointlights[this->_num_pointlights].m_transform = this->rearObjectPtr()->getTransform();
    this->pointlight_parent->giveChild(this->rearObjectPtr());
    this->_num_pointlights += 1;
  }

  if (object_name == "spotlight")
  {
    this->rearObjectPtr()->hasGeometry(false);
    this->rearObjectPtr()->lightsource_components.push_back(EntityComponent(COMPONENT_LIGHTSOURCE, &this->spotlights[this->_num_spotlights]));
    this->spotlights[this->_num_spotlights].m_transform = this->rearObjectPtr()->getTransform();
    this->spotlight_parent->giveChild(this->rearObjectPtr());
    this->_num_spotlights += 1;
  }

  if (object_name == "pointlightcontainer")
  {
    this->pointlight_parent = this->rearObjectPtr();
    this->pointlight_parent->setName("Point Lights");
    this->pointlight_parent->transform_components.clear();
  }

  if (object_name == "spotlightcontainer")
  {
    this->spotlight_parent = this->rearObjectPtr();
    this->spotlight_parent->setName("Spot Lights");
    this->spotlight_parent->transform_components.clear();
  }

}


void SceneGraph::clearScene(void)
{
  this->m_object_instances.clear();
  this->_num_pointlights = 0;
  this->_num_spotlights = 0;
  this->_num_active_pointlights = 0;
  this->_num_active_spotlights = 0;

  this->pointlight_parent = nullptr;
  this->spotlight_parent = nullptr;
}

void SceneGraph::defaultScene(void)
{
  this->clearScene();

  this->newObjectInstance("pointlightcontainer");
  this->newObjectInstance("spotlightcontainer");

  this->newObjectInstance("pointlight");
  this->newObjectInstance("spotlight");
  this->newObjectInstance("spotlight");
}


void SceneGraph::updateLights(void)
{
  this->sorted_pointlights.clear();
  this->sorted_spotlights.clear();

  this->_num_active_pointlights = 0;
  this->_num_active_spotlights = 0;

  int count = 0;

  for (int i=0; i<1; i++)
    if (this->pointlights[i].active)
    {
      this->sorted_pointlights.push_back(this->pointlights[i]);
      this->_num_active_pointlights += 1;
    }
  
  for (int i=0; i<1; i++)
    if (!this->pointlights[i].active)
      this->sorted_pointlights.push_back(this->pointlights[i]);
  

  for (int i=0; i<2; i++)
    if (this->spotlights[i].active)
    {
      this->sorted_spotlights.push_back(&this->spotlights[i]);
      this->_num_active_spotlights += 1;
    }

  for (int i=0; i<2; i++)
    if (!this->spotlights[i].active)
      this->sorted_spotlights.push_back(&this->spotlights[i]);
  
}


void objectToFile(std::ofstream *stream, GameObject *object)
{
  *stream << "#GAMEOBJECT BEGIN" << std::endl;

  *stream << "m_template_name: " << object->getTemplateName() << std::endl;
  *stream << "m_given_name: "    << object->getName() << std::endl;

  *stream << "objectID: " << object->getID() << std::endl;
  if (object->getParent() != nullptr)
    *stream << "parentID: " << object->getParent()->getID() << std::endl;
  else
    *stream << "parentID: -1" << std::endl;

  
  glm::vec3 v;
  v = object->getTransform()->getPos_worldspace();
  *stream << "position: " << v.x << " " << v.y << " " << v.z << std::endl;

  v = *object->getVel();
  *stream << "velocity: " << v.x << " " << v.y << " " << v.z << std::endl;

  glm::quat q = object->getTransform()->orientation;
  *stream << "orientation: " << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << std::endl;

  for (int i=0; i<object->script_components.size(); i++)
  {
    *stream << "#SCRIPT: ";
    *stream << object->script_components[i].script_name << std::endl;
  }

  if (object->lightsource_components.size() >= 1)
  {
    *stream << "#LIGHTSOURCE BEGIN" << std::endl;
    object->lightsource_components[0].toFile(*stream);
    *stream << "#LIGHTSOURCE END" << std::endl;
  }

  *stream << "#GAMEOBJECT END\n" << std::endl;
 
}


void SceneGraph::objectFromFile(std::ifstream &stream, std::string &line)
{

}


void SceneGraph::objectFromFile_headerData(std::ifstream &stream, std::string &line, Player *player)
{
  int objectID = -1, parentID = -1;
  GameObject *object;

  while (getline(stream, line))
  {
    if (line.find("m_template_name") != std::string::npos)
    {
      line.erase(0, std::string("m_template_name").size() + 2);

      if (line == "player")
      {
        this->newObjectInstance("player");
        object = this->rearObjectPtr();
        player->useGameObject(object);
        player->m_gameobject->changePhysState(PHYSICS_FALLING);
      }

      else
      {
        this->newObjectInstance(line);
        object = this->rearObjectPtr();
      }
    }

    else if (line.find("m_given_name") != std::string::npos)
    {
      line.erase(0, std::string("m_given_name").size() + 2);
      object->setName(line);
    }

    else if (line.find("parentID") != std::string::npos)
    {
      line.erase(0, std::string("parentID").size() + 2);
      object->parentID = std::stoi(line);
    }

    else if (line.find("position: ") != std::string::npos)
    {
      line.erase(0, std::string("position: ").size());
      std::stringstream ss;
      ss << line;
      glm::vec3 v;
      int count = 0;
      float n;
      while (ss >> n)
      {
        v[count] = n;
        count += 1;
      }
      *object->getPos() = v;
    }

    else if (line.find("velocity: ") != std::string::npos)
    {
      line.erase(0, std::string("velocity: ").size());
      std::stringstream ss;
      ss << line;
      glm::vec3 v;
      int count = 0;
      float n;
      while (ss >> n)
      {
        v[count] = n;
        count += 1;
      }
      *object->getVel() = v;
    }

    else if (line.find("orientation: ") != std::string::npos)
    {
      line.erase(0, std::string("orientation: ").size());
      std::stringstream ss;
      ss << line;
      glm::quat q;
      int count = 0;
      float n;
      while (ss >> n)
      {
        q[count] = n;
        count += 1;
      }

      object->getTransform()->orientation = q;
    }

    else if (line.find("#SCRIPT: ") != std::string::npos)
    {
      object->script_components.push_back(EntityComponent(COMPONENT_SCRIPT));
      line.erase(0, std::string("#SCRIPT: ").size());
      object->script_components[object->script_components.size()-1].script_name = line;
    }

    else if (line == "#LIGHTSOURCE BEGIN")
      object->lightsource_components[0].fromFile(stream);

    else if (line.find("#GAMEOBJECT END") != std::string::npos)
      return;
  }
}


bool SceneGraph::exportScene(std::string filepath)
{
  std::cout << "exporting scene to: " << filepath << std::endl;

  std::ofstream stream;
  stream.open(filepath);

  for (auto &object: this->m_object_instances)
  {
    objectToFile(&stream, &object);
  }

  stream.close();
}


bool SceneGraph::importScene(std::string filepath, Player *player)
{
  printf("scene_name: %s\n", filepath.c_str());

  this->clearScene();

  std::ifstream stream;
  stream.open(filepath);

  std::string line;

  // First five objects are always lights + player, load those first
  while (getline(stream, line))
  {
    if (line == "#GAMEOBJECT BEGIN")
      this->objectFromFile_headerData(stream, line, player);
  }

  for (auto &object: this->m_object_instances)
  {
    if (object.parentID != -1 && object.m_template_name != "spotlight")
      this->objectPtr(object.parentID)->giveChild(this->objectPtr(object.m_ID), false);
  }

  stream.close();
}

