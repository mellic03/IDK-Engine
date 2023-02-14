#include "../scenegraph.h"
#include "fileutil.h"


void SceneGraph::objectFromFile(std::ifstream &stream, std::string &line, Player *player)
{
  GameObject *object = nullptr;

  while (getline(stream, line))
  {
    if (line.find("<HEADER>") != std::string::npos)
    {
      GameObjectHeader header = FileUtil::FromText::objectheader(stream, object);
      this->newObjectInstance(header.template_name);
      object = this->rearObjectPtr();
      object->parentID = header.parentID;
      object->m_given_name = header.assigned_name;
      *object->getData()->flags()->bits() = header.gameobjectFlags;
      *object->getData()->physData()->flags()->bits() = header.physicsFlags;
      *object->getData()->navData()->flags()->bits() = header.navigationFlags;

      if (object->getObjectType() == GAMEOBJECT_PLAYER)
      {
        this->player_object = object;
        player->useGameObject(object);
      }
    }

    else if (line.find("<TRANSFORM>") != std::string::npos)
      FileUtil::FromText::transform(stream, object->getTransform());


    else if (line.find("<SCRIPTS>") != std::string::npos)
      FileUtil::FromText::scripts(stream, object);

    else if (line.find("<POINTLIGHT>") != std::string::npos)
    {
      FileUtil::FromText::pointlight(stream, object->getComponents()->getPointLightComponent());
    }

    else if (line.find("<SPHERECOLLIDER>") != std::string::npos)
    { 
      FileUtil::FromText::spherecollider(stream, object);
      object->getComponents()->giveComponent(COMPONENT_SPHERE_COLLIDER);
    }

    else if (line.find("<PHYSICS>") != std::string::npos)
    { 
      // object->getData()->flags()->set(GameObjectFlag::PHYSICS, true);
      object->getComponents()->giveComponent(COMPONENT_PHYSICS);
    }


    else if (line.find("<NAVIGATION>") != std::string::npos)
    { 
      FileUtil::FromText::navigation(stream, object);
    }
    // else if (line == "#LIGHTSOURCE BEGIN")
    //   object->lightsource_components[0].fromFile(stream);

    else if (line.find("</GAMEOBJECT>") != std::string::npos)
      return;
  }
}


void SceneGraph::importScene(std::string filepath, Player *player)
{
  this->clearScene();

  printf("Loading scene from: %s\n", filepath.c_str());

  std::ifstream stream;
  stream.open(filepath);

  std::string line;
  while (getline(stream, line))
  {
    if (line == "<GAMEOBJECT>")
      this->objectFromFile(stream, line, player);
  }

  for (auto &object: this->m_object_instances)
  {
    if (object.parentID != -1)
      this->objectPtr(object.parentID)->giveChild(this->objectPtr(object.getID()));
  }

  stream.close();
}