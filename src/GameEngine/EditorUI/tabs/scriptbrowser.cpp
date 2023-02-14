#include "../UIEngine.h"


void EngineUI::scriptBrowser(void)
{
  static bool changed = false;

  fs::path *selected_path = &EngineUI::filebrowser.selected_path;

  ImGui::Begin("Script Browser");
  {
    EngineUI::draw_directory_recursive(fs::current_path()/"LuaScripting", selected_path, &changed);
    if (changed)
    {
      std::ifstream fh;
      fh.open(fs::relative(*selected_path, ".").c_str());
      std::string raw_file = "";
      std::string line;
      while (getline(fh, line))
        raw_file += line + "\n";

      strcpy(EngineUI::script_buffer, raw_file.c_str());

      changed = false;
    }
    ImGui::End();
  }
}
