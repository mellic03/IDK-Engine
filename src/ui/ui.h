#pragma once

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl.h"
#include "../include/imgui/imgui_impl_opengl3.h"

#include "../GraphicsEngine/GraphicsEngine.h"
#include "../scene/scene.h"

#include "UIEngine.h"

namespace fs = std::filesystem;

void draw_main_menu_bar();

void draw_ui(Renderer *ren, int *x, int *y, int *w, int *h);


// Scene tab
//----------------------------------------------------------------------------------
void draw_scene_tab(Renderer *ren, int *selected_instance);
//----------------------------------------------------------------------------------

