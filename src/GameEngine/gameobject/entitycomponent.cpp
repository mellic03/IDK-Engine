#include "entitycomponent.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../ui/UIEngine.h"

namespace fs = std::filesystem;



static void draw_directory_recursive(fs::path pth, fs::path *selected_filepath, bool *changed)
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

      ImGui::Selectable(dir_entry.path().filename().string().c_str(), &s);
      if (ImGui::IsItemClicked())
      {
        *selected_filepath = dir_entry.path();
        *changed = true;
      }
    }
  }

}



EntityComponent::EntityComponent(EntityComponentType component_type)
{
  this->_component_type = component_type;
}


void EntityComponent::draw(GameObject *object)
{
  int selected_pointlight = 0;

  switch (this->_component_type)
  {
    case (COMPONENT_NONE):
      break;

    case (COMPONENT_TRANSFORM):
      if (ImGui::CollapsingHeader("Transform"))
      {
        EngineUI::vec3("Position", object->getPos(), 0.01f);
        EngineUI::vec3("Velocity", object->getVel(), 0.01f);
        EngineUI::vec3("Rotation", object->getRot(), 0.1f);
      }
      break;


    case (COMPONENT_LIGHTSOURCE):

      if (ImGui::CollapsingHeader("Lightsource"))
      {
        ImGui::Checkbox("Enable", &Render::ren.pointlights_on[selected_pointlight]);

        ImGui::ColorEdit3("ambient", (float*)&Render::ren.pointlights[selected_pointlight].ambient);
        ImGui::ColorEdit3("diffuse", (float*)&Render::ren.pointlights[selected_pointlight].diffuse);

        ImGui::SliderFloat("constant", &Render::ren.pointlights[selected_pointlight].constant, 0.0f, 100.0f, "%0.4f", 0);
        ImGui::DragScalar("linear", ImGuiDataType_Float, &Render::ren.pointlights[selected_pointlight].linear,       0.001f, 0);
        ImGui::DragScalar("quadratic", ImGuiDataType_Float, &Render::ren.pointlights[selected_pointlight].quadratic, 0.001f, 0);
        ImGui::DragScalar("bias", ImGuiDataType_Float, &Render::ren.pointlights[selected_pointlight].bias, 0.001f, 0);
      }
      break;


    case (COMPONENT_SCRIPT):
      if (ImGui::CollapsingHeader(std::string("Script        " + this->script_path.filename().string()).c_str()))
      {
        if (ImGui::Button(std::string(this->script_name + ".lua").c_str()))
          ImGui::OpenPopup("Change Script");


        ImGui::SetNextWindowSize({300, 300});
        if (ImGui::BeginPopup("Change Script"))
        {
          bool changed = false;
          draw_directory_recursive(fs::current_path()/"LuaScripting/scripts", &this->script_path, &changed);
          if (changed)
          {
            fs::path filepath = fs::relative(this->script_path, ".");

            this->script_name = filepath.string();
            this->script_name.erase(this->script_name.size() - 4);
          }

          ImGui::EndPopup();
        }

      }
      break;
  }


}


