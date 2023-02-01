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
  PHYSICS     = 0b00000100,
  HIDDEN      = 0b00001000
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

    std::map<std::string, int>          _int_values;
    std::map<std::string, float>        _float_values;
    std::map<std::string, std::string>  _string_values;


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

    inline Navigation::NavData *navData() { return &this->_navigation_data; };
    inline PhysicsData *physData()        { return &this->_physics_data;    };

    
    int intValue(std::string name) { return this->_int_values[name]; };
    void intValue(std::string name, int value) { this->_int_values[name] = value; };

    float floatValue(std::string name) { return this->_float_values[name]; };
    void floatValue(std::string name, float value) { this->_float_values[name] = value; };
};

