#include "gameobject.h"


int *GameObject::getLOD()
{
  return &this->_lod;
}

int GameObject::getLevelsLOD()
{
  return this->_modelLOD->getLevelsLOD();
}

Model *GameObject::getModel(void)
{
  return this->_modelLOD->getLOD(this->_lod);
}

Model *GameObject::getModel(int lod)
{
  return this->_modelLOD->getLOD(lod);
}
