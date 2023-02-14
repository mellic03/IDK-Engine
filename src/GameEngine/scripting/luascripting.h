#pragma once

#include <iostream>
#include <lua.hpp>

#include "luainterface.h"


void luaInit();
void luaMain(Renderer *ren, KeyLog *keylog, std::list<GameObject> *gameobjects);

