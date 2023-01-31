#pragma once

#include "../../../GraphicsEngine/GraphicsEngine.h"
#include "../../state.h"
#include "../../physics.h"
#include "../../computemesh/computemesh.h"

#include "navdata.h"
#include "physicsdata.h"


enum BillboardType {
  BILLBOARD_FIXED,
  BILLBOARD_FOLLOW_CAMERA
};


enum GameObjectType {
  GAMEOBJECT_UNDEFINED,
  GAMEOBJECT_EMPTY,
  GAMEOBJECT_TERRAIN,
  GAMEOBJECT_STATIC,
  GAMEOBJECT_BILLBOARD,
  GAMEOBJECT_ACTOR,
  GAMEOBJECT_PLAYER,
  GAMEOBJECT_LIGHTSOURCE,
  GAMEOBJECT_NUM_TYPES
};


enum class GameObjectFlag: GLuint {
  NONE        = 0b00000000,
  INSTANCED   = 0b00000001,
  ANIMATED    = 0b00000010,
  NAVMESH     = 0b00000100
};


inline GameObjectFlag operator | (GameObjectFlag A, GameObjectFlag B)
{
  return static_cast<GameObjectFlag>(static_cast<GLuint>(A) | static_cast<GLuint>(B));
}

inline GameObjectFlag operator & (GameObjectFlag A, GameObjectFlag B)
{
  return static_cast<GameObjectFlag>(static_cast<GLuint>(A) & static_cast<GLuint>(B));
}


struct GameObjectData {

  private:
    bool _animated = false;

    GameObjectFlag _object_flags = GameObjectFlag::NONE;
    Navigation::NavData _navigation_data;
    PhysicsData _physics_data;


  public:

    GameObjectType gameobject_type = GAMEOBJECT_UNDEFINED;
    LightSourceType lightsource_type = LIGHTSOURCE_NONE;
    BillboardType billboard_type = BILLBOARD_FIXED;
    GameObjectType ui_icon_type = GAMEOBJECT_UNDEFINED;

    GameObjectFlag *getFlags();
    bool getFlag(GameObjectFlag flag);
    void setFlag(GameObjectFlag flag, bool value);

    void setLightSourceType(LightSourceType type);
    void setBillboardType(BillboardType type);

    void setUiIconType(GameObjectType type);
    GameObjectType getUiIconType();

    inline Navigation::NavData *navData();
    inline PhysicsData *physData() { return &this->_physics_data; };

};
