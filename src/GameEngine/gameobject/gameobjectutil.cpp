#include "gameobjectutil.h"


GameObjectFlag GameObjectUtil::objectFlag_fromString(std::string stringdata)
{
  if (stringdata == "INSTANCED")
    return GameObjectFlag::INSTANCED;

  return GameObjectFlag::NONE;
}


GameObjectType GameObjectUtil::objectType_fromString(std::string stringdata)
{
  if (stringdata == "GAMEOBJECT_EMPTY")
    return GAMEOBJECT_EMPTY;

  else if (stringdata == "GAMEOBJECT_TERRAIN")
    return GAMEOBJECT_TERRAIN;

  else if (stringdata == "GAMEOBJECT_STATIC")
    return GAMEOBJECT_STATIC;

  else if (stringdata == "GAMEOBJECT_BILLBOARD")
    return GAMEOBJECT_BILLBOARD;

  else if (stringdata == "GAMEOBJECT_ACTOR")
    return GAMEOBJECT_ACTOR;

  else if (stringdata == "GAMEOBJECT_PLAYER")
    return GAMEOBJECT_PLAYER;

  else if (stringdata == "GAMEOBJECT_LIGHTSOURCE")
    return GAMEOBJECT_LIGHTSOURCE;

  return GAMEOBJECT_UNDEFINED;
}


LightSourceType GameObjectUtil::lightsourceType_fromString(std::string stringdata)
{
  if (stringdata == "LIGHTSOURCE_DIRECTIONAL_LIGHT")
    return LIGHTSOURCE_DIRECTIONAL_LIGHT;

  else if (stringdata == "LIGHTSOURCE_POINT_LIGHT")
    return LIGHTSOURCE_POINT_LIGHT;

  else if (stringdata == "LIGHTSOURCE_SPOT_LIGHT")
    return LIGHTSOURCE_SPOT_LIGHT;

  return LIGHTSOURCE_NONE;
}


BillboardType GameObjectUtil::billboardType_fromString(std::string stringdata)
{
  if (stringdata == "BILLBOARD_FOLLOW_CAMERA")
    return BILLBOARD_FOLLOW_CAMERA;

  return BILLBOARD_FIXED;
}