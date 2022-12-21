#pragma once

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl.h"
#include "../include/imgui/imgui_impl_opengl3.h"

#include "../GraphicsEngine/renderer.h"
#include "../scene/scene.h"


void draw_main_menu_bar(Renderer *ren, Scene *scene);

void draw_dev_ui(Renderer *ren, Scene *scene, int *x, int *y, int *w, int *h);
void import_lighting_config(Renderer *ren);


// Scene tab
//----------------------------------------------------------------------------------
void draw_scene_tab(Renderer *ren, Scene *scene, int *selected_instance);
void draw_properties_menu(Scene *scene, SceneGraph *handler, int selected_instance);
//----------------------------------------------------------------------------------
