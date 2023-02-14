#pragma once

#include <iostream>
#include <lua.hpp>

#include "luainterface.h"


void luaInit();
void luaMain(Renderer *ren, Player *player, std::list<GameObject> *gameobjects);

