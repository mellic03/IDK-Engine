#include "luainterface.h"


void LuaInterface::ToLua::array_bool(bool arrOfBool[], int size, std::string name)
{
  lua_newtable(LuaInterface::L);
  for (int i=0; i<size; i++)
  {
    lua_pushinteger(LuaInterface::L, i+1);
    lua_pushboolean(LuaInterface::L, arrOfBool[i]);
    lua_settable(L, -3);
  }
  lua_setglobal(L, name.c_str());
}


void LuaInterface::ToLua::stdvec_int(std::vector<int> vecOfInt, std::string name)
{
  lua_newtable(LuaInterface::L);

  for (int i=0; i<vecOfInt.size(); i++)
  {
    lua_pushinteger(LuaInterface::L, i+1);
    lua_pushinteger(LuaInterface::L, vecOfInt[i]);
    lua_settable(LuaInterface::L, -3);
  }

  lua_setglobal(LuaInterface::L, name.c_str());
}


void LuaInterface::ToLua::stdvec_bool(std::vector<bool> vecOfBool, std::string name)
{

}


void LuaInterface::ToLua::stdvec_stdstring(std::vector<std::string> vecOfString, std::string name)
{
  lua_newtable(LuaInterface::L);

  for (size_t i=0; i<vecOfString.size(); i++)
  {
    lua_pushinteger(LuaInterface::L, i+1);
    lua_pushstring(LuaInterface::L, vecOfString[i].c_str());
    lua_settable(LuaInterface::L, -3);
  }

  lua_setglobal(LuaInterface::L, name.c_str());
}


void LuaInterface::ToLua::stdvec_vec3(std::vector<glm::vec3> vecOfVec3, std::string name)
{
  lua_newtable(LuaInterface::L);

  for (int i=0; i<vecOfVec3.size(); i++)
  {
    lua_pushinteger(LuaInterface::L, i+1);

    lua_newtable(LuaInterface::L);
    {
      lua_pushnumber(LuaInterface::L, vecOfVec3[i].x);
      lua_setfield(LuaInterface::L, -2, "x");

      lua_pushnumber(LuaInterface::L, vecOfVec3[i].y);
      lua_setfield(LuaInterface::L, -2, "y");

      lua_pushnumber(LuaInterface::L, vecOfVec3[i].z);
      lua_setfield(LuaInterface::L, -2, "z");
    }
  
    lua_settable(LuaInterface::L, -3);
  }

  lua_setglobal(LuaInterface::L, name.c_str());
}


void LuaInterface::ToLua::gameobject(GameObject *object, int objectID)
{
  // for (auto component: object->script_components)
  // {
  //   if (component.script_name != "LuaScripting/scripts/default");
  //   LuaInterface::IDs.push_back(objectID);
  //   LuaInterface::scripts.push_back(component.script_name);
  // }

  // LuaInterface::positions.push_back(object->getTransform()->getPos_worldspace());
  // LuaInterface::velocities.push_back(*object->getTransform()->getVel());
}


void LuaInterface::ToLua::keylog(KeyLog keylog, std::string name)
{
  LuaInterface::ToLua::array_bool(keylog.keys_pressed, SDL_NUM_SCANCODES, name);
}

