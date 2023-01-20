#include "luainterface.h"


lua_State *LuaInterface::L;
Scene *LuaInterface::m_scene;
SceneGraph *LuaInterface::m_scenegraph;
Renderer *LuaInterface::m_renderer;


void LuaInterface::init(Scene *scene, Renderer *renderer)
{
  LuaInterface::m_scene = scene;
  LuaInterface::m_scenegraph = scene->m_scenegraph;
  LuaInterface::m_renderer = renderer;
}


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

