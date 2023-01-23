#include "../luainterface.h"
#include "luaLibrary.h"



extern "C" int setPhysicsState(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *physics_state = lua_tostring(LS, 1);
  Scene::scenegraph.objectPtr(objectID)->changePhysState(physics_state);

  return 0;
}


extern "C" int getPos(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
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

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  *object->getPos() = glm::vec3(x, y, z);

  return 0;
}


extern "C" int getVel(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
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

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  *object->getVel() = glm::vec3(x, y, z);

  return 0;
}


extern "C" int addToVel(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  *object->getVel() += glm::vec3(x, y, z);

  return 0;
}


extern "C" int addRot(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);
  
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  object->getTransform()->addRot(glm::vec3(x, y, z));

  return 0;
}




void register_luaLibrary_gameobject()
{
  lua_register(LuaInterface::L, "CE_GameObject_SetPhysicsState", setPhysicsState);
  lua_register(LuaInterface::L, "CE_GameObject_GetPos", getPos);
  lua_register(LuaInterface::L, "CE_GameObject_SetPos", setPos);
  lua_register(LuaInterface::L, "CE_GameObject_GetVel", getVel);
  lua_register(LuaInterface::L, "CE_GameObject_SetVel", setVel);
  lua_register(LuaInterface::L, "CE_GameObject_AddRot", addRot);
}

