#include "../luainterface.h"
#include "luaLibrary.h"



extern "C" int setGravity(lua_State *LS)
{
  float g = lua_tonumber(LS, 1);
  PhysicsEngine::gravity = g;
  
  return 0;
}




void register_luaLibrary_physics()
{
  lua_register(LuaInterface::L, "CE_Physics_SetGravity", setGravity);
}
