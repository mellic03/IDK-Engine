#include "ui.h"

#include <filesystem>
namespace fs = std::filesystem;


void EngineUI::draw_directory_recursive(fs::path pth, fs::path *selected_filepath, bool *changed)
{
  // Two passes, first showing directories, then files

  for (auto const& dir_entry : fs::directory_iterator{pth}) 
  {
    if (dir_entry.path().has_extension())
      continue;

    if (ImGui::TreeNode(dir_entry.path().filename().string().c_str()))
    {
      draw_directory_recursive(pth / dir_entry.path(), selected_filepath, changed);
      ImGui::TreePop();
    }
  }

  for (auto const& dir_entry : fs::directory_iterator{pth}) 
  {
    if (dir_entry.path().has_extension())
    {
      bool s = (dir_entry.path().compare(*selected_filepath) == 0);

      ImGui::Selectable(dir_entry.path().filename().string().c_str(), &s, ImGuiSelectableFlags_DontClosePopups);
      if (ImGui::IsItemClicked())
      {
        *selected_filepath = dir_entry.path();
        *changed = true;
      }
    }
  }

}