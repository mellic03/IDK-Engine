#pragma once


#include "common_EditorUI.h"

#include "UIEngine.h"

namespace fs = std::filesystem;

void draw_main_menu_bar();

void draw_ui(Renderer *ren, int *x, int *y, int *w, int *h);


// Scene tab
//----------------------------------------------------------------------------------
void draw_scene_tab(Renderer *ren, int *selected_instance);
//----------------------------------------------------------------------------------

