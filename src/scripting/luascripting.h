#pragma once

#include <iostream>
#include <lua.hpp>

#include "../include/glm/glm.hpp"

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../GameEngine/GameEngine.h"

#include "luainterface.h"


void luaInit(Scene *scene, Renderer *renderer);
void luaMain(Renderer *ren, Player *player, std::list<GameObject> *gameobjects);

