#include "gameobjectutil.h"

#define StringLiteral(var) (#var)


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


Navigation::NavState GameObjectUtil::navstate_fromString(std::string stringdata)
{
  if (stringdata == "SEEK")
    return Navigation::NavState::SEEK;

  return Navigation::NavState::NONE;
}


namespace GameObjectUtil {


  namespace FromString {


    PhysicsState physicsState(std::string str)
    {
      if (str == "GROUNDED")
        return PhysicsState::GROUNDED;

      if (str == "FALLING")
        return PhysicsState::FALLING;

      return PhysicsState::NONE;
    };


  };




  namespace ToString {


    std::string gameObjectType(GameObjectType type)
    {
      switch (type)
      {
        default:
        case (GAMEOBJECT_UNDEFINED):    return "GAMEOBJECT_UNDEFINED";
        case (GAMEOBJECT_TERRAIN):      return "GAMEOBJECT_TERRAIN";
        case (GAMEOBJECT_STATIC):       return "GAMEOBJECT_STATIC";
        case (GAMEOBJECT_BILLBOARD):    return "GAMEOBJECT_BILLBOARD";
        case (GAMEOBJECT_ACTOR):        return "GAMEOBJECT_ACTOR";
        case (GAMEOBJECT_PLAYER):       return "GAMEOBJECT_PLAYER";
        case (GAMEOBJECT_LIGHTSOURCE):  return "GAMEOBJECT_LIGHTSOURCE";
      }
    }


  };


};
