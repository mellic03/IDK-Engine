#include "../luainterface.h"
#include "luaLibrary.h"



extern "C" int setMoveForce(lua_State *LS)
{
  float force = lua_tonumber(LS, 1);
  Scene::player.move_speed = force;

  return 0;
}


extern "C" int setJumpForce(lua_State *LS)
{
  float force = lua_tonumber(LS, 1);
  Scene::player.jump_force = force;

  return 0;
}


extern "C" int setFly(lua_State *LS)
{
  bool fly = lua_toboolean(LS, 1);
  Scene::player.fly = fly;

  return 0;
}


extern "C" int getDir(lua_State *LS)
{
  glm::vec3 dir = Render::ren.cam.front;

  lua_newtable(LS);

  lua_pushnumber(LS, dir.x);
  lua_setfield(LS, -2, "x");

  lua_pushnumber(LS, dir.y);
  lua_setfield(LS, -2, "y");

  lua_pushnumber(LS, dir.z);
  lua_setfield(LS, -2, "z");

  return 1;
}


void register_luaLibrary_player()
{
  lua_register(LuaInterface::L, "CE_Player_SetMoveForce", setMoveForce);
  lua_register(LuaInterface::L, "CE_Player_SetJumpForce", setJumpForce);
  lua_register(LuaInterface::L, "CE_Player_SetFly",       setFly);
  lua_register(LuaInterface::L, "CE_Player_GetDir",       getDir);
}
