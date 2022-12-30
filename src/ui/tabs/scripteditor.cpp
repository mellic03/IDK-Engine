#include "../UIEngine.h"


void EngineUI::scriptEditor(void)
{
  ImGui::Begin("Script Editor");
  {
    ImGui::InputTextMultiline("Script", EngineUI::script_buffer, 2048, ImVec2(-1, ImGui::GetContentRegionAvail().y-35.0f), EngineUI::script_flags);

    if (ImGui::Button("Save"))
    {
      std::ofstream fh;
      fh.open(EngineUI::filebrowser.selected_path.string(), std::ios::out);
      fh << script_buffer;
      fh.close();
      LuaInterface::compile();
    }

    ImGui::SameLine();

    if (ImGui::Button("Reload"))
    {
      LuaInterface::compile();
    }
    
    
    ImGui::End();
  }
}
