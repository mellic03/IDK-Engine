#include "../luainterface.h"
#include "luaLibrary.h"



extern "C" int setMoveForce(lua_State *LS)
{
  float force = lua_tonumber(LS, 1);
  LuaInterface::m_scene->player->move_speed = force;

  return 0;
}


extern "C" int setJumpForce(lua_State *LS)
{
  float force = lua_tonumber(LS, 1);
  LuaInterface::m_scene->player->jump_force = force;

  return 0;
}


extern "C" int setFly(lua_State *LS)
{
  bool fly = lua_toboolean(LS, 1);
  LuaInterface::m_scene->player->fly = fly;

  return 0;
}


void register_luaLibrary_player()
{
  lua_register(LuaInterface::L, "CE_Player_SetMoveForce", setMoveForce);
  lua_register(LuaInterface::L, "CE_Player_SetJumpForce", setJumpForce);
  lua_register(LuaInterface::L, "CE_Player_SetFly",       setFly);
}
