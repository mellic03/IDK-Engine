#include "imgui_stl.h"




void ImGui::Dropdown(std::string label, std::vector<std::string> items, std::string *selected, int *index)
{
  const char* combo_preview_value = items[*index].c_str();
  if (ImGui::BeginCombo(label.c_str(), combo_preview_value, 0))
  {
    for (size_t n = 0; n < items.size(); n++)
    {
      const bool is_selected = (*index == n);
      if (ImGui::Selectable(items[n].c_str(), is_selected))
      {
        *selected = items[n];
        *index = n;
      }

      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
}



