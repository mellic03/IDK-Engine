#pragma once

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_sdl.h"
#include "../include/imgui/imgui_impl_opengl3.h"

#include "../GraphicsEngine/renderer.h"
#include "../scene/scene.h"

#include "tabs/tabs.h"
#include "filebrowser.h"



namespace EngineUI {

  extern int selected_objectID;

  extern FileBrowser filebrowser;

};