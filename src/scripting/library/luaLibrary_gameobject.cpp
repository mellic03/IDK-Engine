#include "../luainterface.h"
#include "luaLibrary.h"



extern "C" int setPhysicsState(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *physStateStr = lua_tostring(LS, 1);
  
  PhysicsState physState = GameObjectUtil::FromString::physicsState(physStateStr);
  Scene::scenegraph.objectPtr(objectID)->getData()->physData()->state = physState;

  return 0;
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

extern "C" int getPos_worldspace(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  glm::vec3 pos = object->getTransform()->getPos_worldspace();

  lua_newtable(LS);

  lua_pushnumber(LS, pos.x);
  lua_setfield(LS, -2, "x");

  lua_pushnumber(LS, pos.y);
  lua_setfield(LS, -2, "y");

  lua_pushnumber(LS, pos.z);
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


extern "C" int setPath(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);
  
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  Navigation::NavData *nData = object->getData()->navData();
  nData->setPath(*object->getPos(), glm::vec3(x, y, z), Scene::scenegraph.getNavMeshes());

  return 0;
}


extern "C" int setInt(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *name = lua_tostring(LS, 2);
  int value = lua_tointeger(LS, 3);
  
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  object->getData()->intValue(std::string(name), value);

  return 0;
}

extern "C" int getInt(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *name = lua_tostring(LS, 2);

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  int value = object->getData()->intValue(std::string(name));

  lua_pushinteger(LS, value);

  return 1;
}


extern "C" int setFloat(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *name = lua_tostring(LS, 2);
  float value = lua_tonumber(LS, 3);
  
  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  object->getData()->floatValue(std::string(name), value);

  return 0;
}

extern "C" int getFloat(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *name = lua_tostring(LS, 2);

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  int value = object->getData()->intValue(std::string(name));

  lua_pushinteger(LS, value);

  return 1;
}


extern "C" int setAnimation(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  const char *animation_name = lua_tostring(LS, 2);

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  object->getAnimationController()->setActiveAnimation(std::string(animation_name));

  return 0;
}


extern "C" int getAnimation(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  const char *animation_name = object->getAnimationController()->getAnimation()->getName().c_str();

  lua_pushstring(LS, animation_name);

  return 1;
}


extern "C" int getNavState(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  std::string navState = GameObjectUtil::ToString::navState(object->getData()->navData()->state);

  lua_pushstring(LS, navState.c_str());

  return 1;
}


extern "C" int setParent(lua_State *LS)
{
  int objectID_child = lua_tointeger(LS, 1) - 1;
  int objectID_parent = lua_tointeger(LS, 2) - 1;

  GameObject *child = Scene::scenegraph.objectPtr(objectID_child);
  GameObject *parent = Scene::scenegraph.objectPtr(objectID_parent);

  parent->giveChild(child);

  return 0;
}


extern "C" int clearParent(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  object->clearParent();

  return 0;
}


extern "C" int getParentID(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  GameObject *object = Scene::scenegraph.objectPtr(objectID);

  int parentID = -1;

  if (object->getParent() != nullptr)
    parentID = object->getParent()->getID() + 1;

  lua_pushinteger(LS, parentID);

  return 1;
}



extern "C" int inBoundingBox(lua_State *LS)
{
  int objectID = lua_tointeger(LS, 1) - 1;
  float x = lua_tonumber(LS, 2);
  float y = lua_tonumber(LS, 3);
  float z = lua_tonumber(LS, 4);

  GameObject *object = Scene::scenegraph.objectPtr(objectID);
  glm::vec3 pos = glm::vec3(x, y, z);


  // Perform test
  //----------------------------------------------------------------------------
  glm::mat4 model = object->getTransform()->getModelMatrix();

  glm::vec3 center = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec3 dx = glm::normalize(model * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
  glm::vec3 dy = glm::normalize(model * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
  glm::vec3 dz = glm::normalize(model * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
 
  glm::vec3 half = glm::vec3(
    glm::distance(model * glm::vec4( -1.0f,  0.0f,  0.0f, 1.0f), model * glm::vec4( +1.0f,  0.0f,  0.0f, 1.0f)) / 2.0f,
    glm::distance(model * glm::vec4(  0.0f, -1.0f,  0.0f, 1.0f), model * glm::vec4(  0.0f, +1.0f,  0.0f, 1.0f)) / 2.0f,
    glm::distance(model * glm::vec4(  0.0f,  0.0f, -1.0f, 1.0f), model * glm::vec4(  0.0f,  0.0f, +1.0f, 1.0f)) / 2.0f
  );

  glm::vec3 d = pos - center;
  bool result = abs(glm::dot(d, dx)) <= half.x &&
                abs(glm::dot(d, dy)) <= half.y &&
                abs(glm::dot(d, dz)) <= half.z;
  //----------------------------------------------------------------------------


  lua_pushboolean(LS, result);

  return 1;
}


void register_luaLibrary_gameobject()
{
  lua_register(LuaInterface::L, "CE_GameObject_SetPhysicsState", setPhysicsState);
  
  lua_register(LuaInterface::L, "CE_GameObject_SetPos", setPos);
  lua_register(LuaInterface::L, "CE_GameObject_GetPos", getPos);
  lua_register(LuaInterface::L, "CE_GameObject_GetPos_worldspace", getPos_worldspace);

  lua_register(LuaInterface::L, "CE_GameObject_SetVel", setVel);
  lua_register(LuaInterface::L, "CE_GameObject_GetVel", getVel);

  lua_register(LuaInterface::L, "CE_GameObject_AddRot", addRot);


  lua_register(LuaInterface::L, "CE_GameObject_SetPath", setPath);


  lua_register(LuaInterface::L, "CE_GameObject_SetInt",  setInt);
  lua_register(LuaInterface::L, "CE_GameObject_GetInt",  getInt);

  lua_register(LuaInterface::L, "CE_GameObject_SetFloat",  setFloat);
  lua_register(LuaInterface::L, "CE_GameObject_GetFloat",  getFloat);

  lua_register(LuaInterface::L, "CE_GameObject_SetAnimation",  setAnimation);
  lua_register(LuaInterface::L, "CE_GameObject_GetAnimation",  getAnimation);



  lua_register(LuaInterface::L, "CE_GameObject_GetNavState",  getNavState);
  
  
  lua_register(LuaInterface::L, "CE_GameObject_SetParent",  setParent);
  lua_register(LuaInterface::L, "CE_GameObject_ClearParent",  clearParent);
  lua_register(LuaInterface::L, "CE_GameObject_GetParentID",  getParentID);


  lua_register(LuaInterface::L, "CE_GameObject_InBoundingBox",  inBoundingBox);
}

