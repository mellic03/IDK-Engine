#pragma once


#include "gameobject.h"



namespace GameObjectUtil
{
  GameObjectType objectType_fromString(std::string stringdata);
  LightSourceType lightsourceType_fromString(std::string stringdata);

  InstancingType instancingType_fromString(std::string stringdata);
  BillboardType  billboardType_fromString(std::string stringdata);
}



