#include "../luainterface.h"
#include "luaLibrary.h"


extern "C" int setBloomMinResolution(lua_State *LS)
{
  int min_resolution = lua_tointeger(LS, 1);
  LuaInterface::m_renderer->bloom.min_downsample_resolution = min_resolution;

  return 0;
}


extern "C" int setBloomThreshold(lua_State *LS)
{
  float threshold = lua_tonumber(LS, 1);
  LuaInterface::m_renderer->bloom.threshold = threshold;  

  return 0;
}


extern "C" int setBloomStrength(lua_State *LS)
{
  float strength = lua_tonumber(LS, 1);
  LuaInterface::m_renderer->bloom.strength = strength;  

  return 0;
}




void register_luaLibrary_lighting()
{
  lua_register(LuaInterface::L, "CE_Lighting_SetBloomMinResolution", setBloomMinResolution);
  lua_register(LuaInterface::L, "CE_Lighting_SetBloomThreshold", setBloomThreshold);
  lua_register(LuaInterface::L, "CE_Lighting_SetBloomStrength", setBloomStrength);
}

