#include "luainterface.h"



extern "C" int getPos(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  GameObject *object = LuaInterface::scenegraph->objectPtr(objectID);
  glm::vec3 pos = *object->getPos();

  lua_newtable(LS);

  lua_pushnumber(LS, pos.x);
  lua_setfield(LS, -2, "x");

  lua_pushnumber(LS, pos.y);
  lua_setfield(LS, -2, "y");

  lua_pushnumber(LS, pos.z);
  lua_setfield(LS, -2, "z");

  return 1;
}


extern "C" int setPos(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);

  GameObject *object = LuaInterface::scenegraph->objectPtr(objectID);
  *object->getPos() = glm::vec3(x, y, z);

  return 0;
}


extern "C" int getVel(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  GameObject *object = LuaInterface::scenegraph->objectPtr(objectID);
  glm::vec3 vel = *object->getVel();

  lua_newtable(LS);

  lua_pushnumber(LS, vel.x);
  lua_setfield(LS, -2, "x");

  lua_pushnumber(LS, vel.y);
  lua_setfield(LS, -2, "y");

  lua_pushnumber(LS, vel.z);
  lua_setfield(LS, -2, "z");

  return 1;
}


extern "C" int setVel(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);

  GameObject *object = LuaInterface::scenegraph->objectPtr(objectID);
  *object->getVel() = glm::vec3(x, y, z);

  return 0;
}


extern "C" int addToVel(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);

  GameObject *object = LuaInterface::scenegraph->objectPtr(objectID);
  *object->getVel() += glm::vec3(x, y, z);

  return 0;
}


void register_library(void)
{
  lua_register(LuaInterface::L, "getPos", getPos);
  lua_register(LuaInterface::L, "setPos", setPos);
  lua_register(LuaInterface::L, "getVel", getVel);
  lua_register(LuaInterface::L, "setVel", setVel);
}
