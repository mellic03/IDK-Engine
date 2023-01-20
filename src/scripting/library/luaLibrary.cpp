#include "../luainterface.h"

#include "luaLibrary.h"


void register_luaLibrary(void)
{
  register_luaLibrary_gameobject();
  register_luaLibrary_lighting();
  register_luaLibrary_physics();
  register_luaLibrary_player();
  register_luaLibrary_scene();
}
