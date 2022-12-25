#include "UIEngine.h"


int EngineUI::selected_objectID = 0;
FileBrowser EngineUI::filebrowser;


void EngineUI::vec3(std::string name, glm::vec3 *data, float step)
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
        data->x = 0.0f;
      ImGui::SameLine();
      ImGui::DragFloat("##X", &data->x, step, 0, 0, (data->x > -1.0f && data->x < 1.0f) ? "%0.3f" : "%0.1f");

      ImGui::TableNextColumn();
      if (ImGui::Button("Y", button_size))
        data->y = 0.0f;
      ImGui::SameLine();
      ImGui::DragFloat("##Y", &data->y, step, 0, 0, (data->y > -1.0f && data->y < 1.0f) ? "%0.3f" : "%0.1f");

      ImGui::TableNextColumn();
      if (ImGui::Button("Z", button_size))
        data->z = 0.0f;
      ImGui::SameLine();
      ImGui::DragFloat("##Z", &data->z, step, 0, 0, (data->z > -1.0f && data->z < 1.0f) ? "%0.3f" : "%0.1f");


    ImGui::PopStyleVar();

    ImGui::EndTable();
  }

  ImGui::PopStyleVar();
}
