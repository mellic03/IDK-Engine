#pragma once

#include <iostream>
#include <lua.hpp>
#include <LuaCpp.hpp>

#include "../include/glm/glm.hpp"

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../GameEngine/GameEngine.h"

#include "luainterface.h"

using namespace LuaCpp;
using namespace LuaCpp::Registry;
using namespace LuaCpp::Engine;






void luaTest(void);

void luaInit(void);
void luaMain(Renderer *ren, Player *player, std::list<GameObject> *gameobjects);