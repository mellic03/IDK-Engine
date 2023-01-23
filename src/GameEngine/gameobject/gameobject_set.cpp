#include "gameobject.h"





void GameObject::setID(int id)
{
  this->header.objectID = id;
}


void GameObject::setName(std::string name)
{
  this->m_given_name = name;
}


void GameObject::setTemplateName(std::string name)
{
  this->m_template_name = name;
}


void GameObject::setObjectType(GameObjectType type)
{
  this->_data.gameobject_type = type;
  this->_data.ui_icon_type = type;
}


void GameObject::setModelLOD(ModelLOD *modelLOD)
{
  this->_modelLOD = modelLOD;
}