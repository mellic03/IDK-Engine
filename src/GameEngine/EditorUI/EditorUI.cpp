#include "EditorUI.h"


void EditorUI::init(SDL_Window *window, void *gl_context)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 440");
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-VariableFont_wdth,wght.ttf", 18.0f);

  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
  ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
  io.Fonts->AddFontFromFileTTF( "src/GameEngine/EditorUI/font/fontawesome-webfont.ttf", 16.0f, &icons_config, icons_ranges );

  ImGui::StyleColorsLight();
}


