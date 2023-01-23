#include "UIEngine.h"

int EngineUI::selected_objectID = 0;
FileBrowser EngineUI::filebrowser;

char EngineUI::script_buffer[4096];
ImGuiInputFlags EngineUI::script_flags = ImGuiInputTextFlags_AllowTabInput;

std::stringstream EngineUI::string_stream;


void EngineUI::dragVec3(std::string name, glm::vec3 *data, float min, float max, float speed, const char *format, float default_value)
{
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

  ImGui::BeginTable(name.c_str(), 4);
  {
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::Text(name.c_str());

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

      float line_height = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
      ImVec2 button_size = { line_height + 3.0f, line_height };

      ImGui::TableNextColumn();
      if (ImGui::Button("X", button_size))
        data->x = default_value;
      ImGui::SameLine();
      ImGui::DragFloat("##X", &data->x, speed, min, max, format);
      
      ImGui::TableNextColumn();
      if (ImGui::Button("Y", button_size))
        data->y = default_value;
      ImGui::SameLine();
      ImGui::DragFloat("##Y", &data->y, speed, min, max, format);

      ImGui::TableNextColumn();
      if (ImGui::Button("Z", button_size))
        data->z = default_value;
      ImGui::SameLine();
      ImGui::DragFloat("##Z", &data->z, speed, min, max, format);

    ImGui::PopStyleVar();

    ImGui::EndTable();
  }

  ImGui::PopStyleVar();
}


void EngineUI::bitFlagCheckBox(const char *label, unsigned char flag, unsigned char *bits)
{
  bool active = flag & *bits;

  ImGui::Checkbox(label, &active);

  if (active)
    *bits = (flag | *bits);
  else
    *bits = (~flag & *bits);

}


std::string EngineUI::getObjectIcon(GameObjectType type)
{
  static constexpr char icons[8][4] = {
    ICON_FA_LIGHTBULB_O,
    ICON_FA_CUBE,
    ICON_FA_TREE,
    ICON_FA_BUILDING,
    ICON_FA_TREE,
    ICON_FA_USER_O,
    ICON_FA_LIGHTBULB_O,
    ICON_FA_LIGHTBULB_O,
  };

  return icons[type];
}
