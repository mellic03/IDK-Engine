#include "../luainterface.h"
#include "luaLibrary.h"



extern "C" int loadScene(lua_State *LS)
{
  const char *scene_name = lua_tostring(LS, 1);
  LuaInterface::m_scenegraph->importScene("assets/scenes/" + std::string(scene_name), LuaInterface::m_scene->player);

  return 0;
}


extern "C" int clearColor(lua_State *LS)
{
  float r = lua_tonumber(LS, 1);
  float g = lua_tonumber(LS, 2);
  float b = lua_tonumber(LS, 3);

  LuaInterface::m_scene->clearColor(glm::vec3(r, g, b) / 255.0f);

  return 0;
}


void register_luaLibrary_scene()
{
  lua_register(LuaInterface::L, "CE_Scene_LoadScene", loadScene);
  lua_register(LuaInterface::L, "CE_Scene_ClearColor", clearColor);
}

