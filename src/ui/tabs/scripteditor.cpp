#include "../UIEngine.h"


static char script_buffer[2048];

void EngineUI::scriptEditor()
{
  static bool changed = false;

  fs::path *selected_path = &EngineUI::filebrowser.selected_path;

  ImGui::Begin("Script");
  {
    draw_directory_recursive(fs::current_path()/"LuaScripting", selected_path, &changed);
    if (changed)
    {
      std::ifstream fh;
      fh.open(fs::relative(*selected_path, ".").c_str());
      std::string raw_file = "";
      std::string line;
      while (getline(fh, line))
        raw_file += line + "\n";

      strcpy(script_buffer, raw_file.c_str());

      changed = false;
    }
    ImGui::End();
  }
}
