#include "luainterface.h"


lua_State *LuaInterface::L;
// std::vector<int> LuaInterface::IDs;
// std::vector<std::string> LuaInterface::scripts;
// std::vector<glm::vec3> LuaInterface::positions;
// std::vector<glm::vec3> LuaInterface::velocities;




void LuaInterface::compile(void)
{
  LuaInterface::L = luaL_newstate();
  luaL_openlibs(LuaInterface::L);
  int status = luaL_loadfile(LuaInterface::L, "LuaScripting/main.lua");
  lua_pcall(LuaInterface::L, 0, 0, 0);
}

void LuaInterface::begin(void)
{
  // LuaInterface::IDs.clear();
  // LuaInterface::scripts.clear();
  // LuaInterface::positions.clear();
  // LuaInterface::velocities.clear();

}

void LuaInterface::execute(void)
{
  lua_getglobal(L, "Main");
  if (lua_isfunction(L, -1))
  {
    // printf("Is function\n");
  }
  else
  {
    printf("NOT A FUNCTION!!\n");
  }

	if (lua_pcall(L, 0, LUA_MULTRET, 0) == 0)
  {
    // std::cout << "Thing called all good" << std::endl;
  }

  else
  {
    std::cout << "Bad bad bad" << std::endl;
    // lua_pop(L, 1);
  }

}

