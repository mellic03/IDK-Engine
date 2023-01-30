#pragma once


#include "gameobject.h"



namespace GameObjectUtil
{
  GameObjectFlag objectFlag_fromString(std::string stringdata);

  GameObjectType objectType_fromString(std::string stringdata);
  LightSourceType lightsourceType_fromString(std::string stringdata);

  BillboardType  billboardType_fromString(std::string stringdata);
}



