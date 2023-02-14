#pragma once

#include "gameobject.h"



namespace GameObjectUtil
{
  GameObjectFlag objectFlag_fromString(std::string str);

  GameObjectType objectType_fromString(std::string str);
  LightSourceType lightsourceType_fromString(std::string str);

  BillboardType  billboardType_fromString(std::string str);

  Navigation::NavState navstate_fromString(std::string str);


  namespace FromString {
    PhysicsState physicsState(std::string str);
  };


  namespace ToString {
    std::string gameObjectType(GameObjectType type);
    std::string physicsState(PhysicsState state);
    std::string navState(Navigation::NavState state);
  };

}



