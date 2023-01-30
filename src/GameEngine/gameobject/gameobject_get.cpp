#include "gameobject.h"


CullingData *GameObject::getCullingData()
{
  return &this->_culling_data;
}


LODData *GameObject::getLODData()
{
  return &this->_lod_data;
}

int *GameObject::getLOD_value()
{
  return &this->_lod_data.level_of_detail;
}

int GameObject::getLevelsLOD()
{
  return this->_modelLOD->getLevelsLOD();
}

ModelLOD *GameObject::getModelLOD()
{
  return this->_modelLOD;
}

Model *GameObject::getModel(void)
{
  if (this->getLODData()->override_global_lod)
    return this->_modelLOD->getLOD_model_override(this->getLODData()->level_of_detail);
  else
    return this->_modelLOD->getLOD_model(this->_lod_data.level_of_detail);
}

Model *GameObject::getModel(int lod)
{
  return this->_modelLOD->getLOD_model(lod);
}


EntityComponents *GameObject::getComponents()
{
  return &this->_entity_components;
}



Animation::AnimationController *GameObject::getAnimationController()
{
  return &this->_animation_controller;
}

Animation::Animation *GameObject::getAnimation()
{
  return this->_animation_controller.getAnimation();
}

Animation::Animation *GameObject::getAnimation(std::string animation_name)
{
  return this->_animation_controller.getAnimation(animation_name);
}


