#include "gameobjectdata.h"




GameObjectFlag *GameObjectData::getFlags()
{
  return &this->_object_flags;
}

bool GameObjectData::getFlag(GameObjectFlag flag)
{
  return static_cast<GLuint>(this->_object_flags) & static_cast<GLuint>(flag);
}

void GameObjectData::setFlag(GameObjectFlag flag, bool value)
{
  GLuint mask  = static_cast<GLuint>(flag);
  GLuint flags = static_cast<GLuint>(this->_object_flags);

  if (value)
    flags |= mask;
  else
    flags &= ~mask;
  
  this->_object_flags = static_cast<GameObjectFlag>(flags);
}


void GameObjectData::setLightSourceType(LightSourceType type)   { this->lightsource_type = type; }
void GameObjectData::setBillboardType(BillboardType type)       { this->billboard_type   = type; }

void GameObjectData::setUiIconType(GameObjectType type)         { this->ui_icon_type = type; }
GameObjectType GameObjectData::getUiIconType()                  { return this->ui_icon_type; }



