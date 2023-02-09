#pragma once

#include "../../../GraphicsEngine/GraphicsEngine.h"
#include "../../state.h"
#include "../../util.h"
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
  NONE        = 0,
  INSTANCED   = 1 << 0,
  ANIMATED    = 1 << 1,
  PHYSICS     = 1 << 2,
  HIDDEN      = 1 << 3,
  GEOMETRY    = 1 << 4
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

    BitFlag<GLuint, GameObjectFlag> _flags;

    Navigation::NavData _navigation_data;
    PhysicsData _physics_data;

    std::map<std::string, int>          _int_values;
    std::map<std::string, float>        _float_values;
    std::map<std::string, std::string>  _string_values;


  public:

    GameObjectType gameobject_type = GAMEOBJECT_UNDEFINED;
    LightSourceType lightsource_type = LIGHTSOURCE_NONE;
    BillboardType billboard_type = BILLBOARD_FIXED;
    GameObjectType ui_icon_type = GAMEOBJECT_UNDEFINED;

    BitFlag<GLuint, GameObjectFlag> *flags() { return &this->_flags; };

    void setLightSourceType(LightSourceType type);
    void setBillboardType(BillboardType type);

    void setUiIconType(GameObjectType type);
    GameObjectType getUiIconType();

    inline Navigation::NavData *navData() { return &this->_navigation_data; };
    inline PhysicsData *physData()        { return &this->_physics_data;    };

    
    int intValue(std::string name) { return this->_int_values[name]; };
    void intValue(std::string name, int value) { this->_int_values[name] = value; };

    float floatValue(std::string name) { return this->_float_values[name]; };
    void floatValue(std::string name, float value) { this->_float_values[name] = value; };
};

