#include "gameobjectutil.h"


GameObjectType GameObjectUtil::objectType_fromString(std::string stringdata)
{
  if (std::string(stringdata) == "GAMEOBJECT_TERRAIN")
    return GAMEOBJECT_TERRAIN;

  else if (std::string(stringdata) == "GAMEOBJECT_STATIC")
    return GAMEOBJECT_STATIC;

  else if (std::string(stringdata) == "GAMEOBJECT_BILLBOARD")
    return GAMEOBJECT_BILLBOARD;

  else if (std::string(stringdata) == "GAMEOBJECT_ACTOR")
    return GAMEOBJECT_ACTOR;

  else if (std::string(stringdata) == "GAMEOBJECT_PLAYER")
    return GAMEOBJECT_PLAYER;

  else if (std::string(stringdata) == "GAMEOBJECT_LIGHTSOURCE")
    return GAMEOBJECT_LIGHTSOURCE;

  return GAMEOBJECT_UNDEFINED;
}



InstancingType GameObjectUtil::instancingType_fromString(std::string stringdata)
{
  if (std::string(stringdata) == "INSTANCING_ON")
    return INSTANCING_ON;

  return INSTANCING_OFF;
}


BillboardType GameObjectUtil::billboardType_fromString(std::string stringdata)
{
  if (std::string(stringdata) == "BILLBOARD_FOLLOW_CAMERA")
    return BILLBOARD_FOLLOW_CAMERA;

  return BILLBOARD_FIXED;
}