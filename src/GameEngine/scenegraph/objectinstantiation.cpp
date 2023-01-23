#include "scenegraph.h"




void SceneGraph::newObjectInstance_empty(GameObject *objectptr)
{

}


void SceneGraph::newObjectInstance_terrain(GameObject *objectptr)
{
  objectptr->entity_components.giveComponent(COMPONENT_TERRAIN);
  // objectptr->terrain_components.push_back(EntityComponent(COMPONENT_TERRAIN));
  // objectptr->terrain_components[0].terrain_component.generateGrassPositions(objectptr->m_model->m_meshes[0].vertices);

}


void SceneGraph::newObjectInstance_static(GameObject *objectptr)
{
  
}


void SceneGraph::newObjectInstance_billboard(GameObject *objectptr)
{
  if (objectptr->getData()->instancing_type == INSTANCING_ON)
  {
    this->addInstanceData(objectptr->getTemplateName(), objectptr->m_model, objectptr->getTransform());
  }
}


void SceneGraph::newObjectInstance_actor(GameObject *objectptr)
{
  
}


void SceneGraph::newObjectInstance_lightsource(GameObject *objectptr)
{
  LightSourceType lightsource_type = objectptr->getData()->lightsource_type;

  switch (lightsource_type)
  {
    case (LIGHTSOURCE_NONE):
      printf("wtf?\n");
      break;

    case (LIGHTSOURCE_DIRECTIONAL_LIGHT):
      break;

    case (LIGHTSOURCE_POINT_LIGHT):
      objectptr->entity_components.givePointLightComponent(&this->pointlights[this->_num_pointlights]);
      this->pointlights[this->_num_pointlights].m_transform = objectptr->getTransform();

      Render::ren.genDepthCubemap(&this->pointlights[this->_num_pointlights].FBO, &this->pointlights[this->_num_pointlights].depthCubemap);

      this->_num_pointlights += 1;
      break;

    case (LIGHTSOURCE_SPOT_LIGHT):
      objectptr->entity_components.giveSpotLightComponent(&this->spotlights[this->_num_spotlights]);
      this->spotlights[this->_num_spotlights].m_transform = objectptr->getTransform();
      this->_num_spotlights += 1;
      break;
  }
}




void SceneGraph::newObjectInstance(std::string object_name, Transform *transform)
{
  GameObject *templateptr = this->templatePtr(object_name);
  if (templateptr == nullptr)
  {
    printf("Null object template pointer for \"%s\" (SceneGraph::newObjectInstance)\n", object_name.c_str());
    exit(1);
  }

  LightSourceType lightsource_type = templateptr->getData()->lightsource_type;
  if (lightsource_type == LIGHTSOURCE_POINT_LIGHT && this->_num_pointlights >= MAX_POINTLIGHTS)
    return;
  else if (lightsource_type == LIGHTSOURCE_SPOT_LIGHT && this->_num_spotlights >= MAX_SPOTLIGHTS)
    return;


  if (templateptr->getData()->instancing_type == INSTANCING_ON)
  {
    this->addInstanceData(templateptr->getTemplateName(), templateptr->m_model, transform);
    return;
  }


  GameObject newobj = *templateptr;
  newobj.m_parent = nullptr;
  *newobj.getTransform() = *transform;
  newobj.setName(object_name);
  newobj.setID(this->m_object_instances.size());
  this->m_object_instances.push_back(newobj);


  GameObject *objectptr = this->rearObjectPtr();
  GameObjectType object_type = objectptr->getObjectType();


  this->_object_instances_by_type[object_type].push_back(objectptr);
  this->m_selectable_instances.push_back(objectptr);
  

  objectptr->entity_components.giveComponent(COMPONENT_TRANSFORM);

  switch (object_type)
  {
    case (GAMEOBJECT_UNDEFINED):
      printf("GAMEOBJECT_UNDEFINED: %s\n", objectptr->getTemplateName().c_str());
      break;

    case (GAMEOBJECT_EMPTY):
      this->newObjectInstance_empty(objectptr);
      break;

    case (GAMEOBJECT_TERRAIN):
      this->newObjectInstance_terrain(objectptr);
      break;

    case (GAMEOBJECT_STATIC):
      this->newObjectInstance_static(objectptr);
      break;

    case (GAMEOBJECT_BILLBOARD):
      this->newObjectInstance_billboard(objectptr);
      break;

    case (GAMEOBJECT_ACTOR):
      this->newObjectInstance_actor(objectptr);
      break;

    case (GAMEOBJECT_PLAYER):
      this->player_object = objectptr;
      break;

    case (GAMEOBJECT_LIGHTSOURCE):
      this->newObjectInstance_lightsource(objectptr);
      break;
  }

}

