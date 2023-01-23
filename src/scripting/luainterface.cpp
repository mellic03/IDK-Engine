#include "luainterface.h"

lua_State *LuaInterface::L;

void LuaInterface::compile(void)
{
  LuaInterface::L = luaL_newstate();
  register_luaLibrary();
  luaL_openlibs(LuaInterface::L);
  int status = luaL_loadfile(LuaInterface::L, "LuaScripting/main.lua");


  lua_pcall(LuaInterface::L, 0, 0, 0);
  printf("Lua compiled\n");
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

