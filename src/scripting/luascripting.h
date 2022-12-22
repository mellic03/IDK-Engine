#pragma once

#include <iostream>
#include <lua.hpp>
#include <LuaCpp.hpp>

#include "../GameEngine/GameEngine.h"
#include "../include/glm/glm.hpp"
#include "luainterface.h"

using namespace LuaCpp;
using namespace LuaCpp::Registry;
using namespace LuaCpp::Engine;






void luaTest(void);

void luaInit(void);
void luaMain(std::list<GameObject> *gameobjects);