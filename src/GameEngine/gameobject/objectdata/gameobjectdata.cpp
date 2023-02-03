#include "gameobjectdata.h"


void GameObjectData::setLightSourceType(LightSourceType type)   { this->lightsource_type = type; }
void GameObjectData::setBillboardType(BillboardType type)       { this->billboard_type   = type; }

void GameObjectData::setUiIconType(GameObjectType type)         { this->ui_icon_type = type; }
GameObjectType GameObjectData::getUiIconType()                  { return this->ui_icon_type; }



