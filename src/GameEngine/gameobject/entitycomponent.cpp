#include "entitycomponent.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../ui/UIEngine.h"

namespace fs = std::filesystem;


EntityComponent::EntityComponent(EntityComponentType component_type)
{
  this->_component_type = component_type;
}

EntityComponent::EntityComponent(EntityComponentType component_type, PointLight *pointlight)
{
  this->_component_type = component_type;
  this->_pointlight = pointlight;
  this->diffuse = &pointlight->diffuse;
}

EntityComponent::EntityComponent(EntityComponentType component_type, SpotLight *spotlight)
{
  this->_component_type = component_type;
  this->_spotlight = spotlight;
  this->diffuse = &spotlight->diffuse;
}


void EntityComponent::_draw_pointlight(void)
{
  if (ImGui::CollapsingHeader("Pointlight"))
  {
    ImGui::Checkbox("Enable", &this->_pointlight->active);

    ImGui::ColorEdit3("ambient", (float*)&this->_pointlight->ambient);
    ImGui::ColorEdit3("diffuse", (float*)&this->_pointlight->diffuse);

    ImGui::SliderFloat("constant", &this->_pointlight->constant, 0.0f, 100.0f, "%0.4f", 0);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &this->_pointlight->linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &this->_pointlight->quadratic, 0.001f, 0);
    ImGui::DragScalar("bias", ImGuiDataType_Float, &this->_pointlight->bias, 0.001f, 0);
  }
}


void EntityComponent::_draw_spotlight(void)
{
  if (ImGui::CollapsingHeader("Spotlight"))
  {
    ImGui::Checkbox("Enable", &this->_spotlight->active);

    ImGui::ColorEdit3("ambient", (float*)&this->_spotlight->ambient);
    ImGui::ColorEdit3("diffuse", (float*)&this->_spotlight->diffuse);

    ImGui::SliderFloat("constant", &this->_spotlight->constant, 0.0f, 100.0f, "%0.4f", 0);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &this->_spotlight->linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &this->_spotlight->quadratic, 0.001f, 0);

    ImGui::DragScalar("intensity", ImGuiDataType_Float, &this->_spotlight->intensity, 0.05f, 0);
    ImGui::DragScalar("inner cutoff", ImGuiDataType_Float, &this->_spotlight->inner_cutoff, 0.05f, 0);
    ImGui::DragScalar("outer cutoff", ImGuiDataType_Float, &this->_spotlight->outer_cutoff, 0.05f, 0);
    ImGui::DragScalar("fov", ImGuiDataType_Float, &this->_spotlight->fov, 0.05f, 0);
  }
}


void EntityComponent::_draw_lightsource(void)
{
  if (this->_pointlight != nullptr)
    this->_draw_pointlight();
  else if (this->_spotlight != nullptr)
    this->_draw_spotlight();
}


void EntityComponent::_draw_transform(GameObject *object)
{
  char buffer[128];
  sprintf(buffer, "Transform ##%d", object->getID());
  if (ImGui::CollapsingHeader(buffer))
  {
    EngineUI::vec3("Position", object->getPos(), 0.01f);
    EngineUI::vec3("Velocity", object->getVel(), 0.01f);

    glm::quat q = object->getTransform()->orientation;
    glm::vec3 drot1 = glm::degrees(glm::vec3(glm::pitch(q), glm::yaw(q), glm::roll(q)));
    glm::vec3 drot2 = drot1;
    EngineUI::vec3("Rotation", &drot1, 0.1f);
    object->getTransform()->addRot(glm::radians(drot1 - drot2));
  }
}


void EntityComponent::_draw_script(GameObject *object)
{
  this->script_changed = false;
  if (ImGui::CollapsingHeader(std::string("Script        " + this->script_path.filename().string()).c_str()))
  {
    if (ImGui::Button(std::string(this->script_name + ".lua").c_str()))
      ImGui::OpenPopup("Change Script");


    ImGui::SetNextWindowSize({300, 300});
    if (ImGui::BeginPopup("Change Script"))
    {
      bool changed = false;
      EngineUI::draw_directory_recursive(fs::current_path()/"LuaScripting/scripts", &this->script_path, &changed);
      if (changed)
      {
        this->script_changed = true;
        fs::path filepath = fs::relative(this->script_path, ".");

        this->script_name = filepath.string();
        this->script_name.erase(this->script_name.size() - 4);
      }
      ImGui::EndPopup();
    }
  }
}




void EntityComponent::draw(GameObject *object)
{
  switch (this->_component_type)
  {
    case (COMPONENT_NONE):
      break;

    case (COMPONENT_TRANSFORM):
      this->_draw_transform(object);
      break;


    case (COMPONENT_LIGHTSOURCE):
      this->_draw_lightsource();
      break;


    case (COMPONENT_SCRIPT):
      this->_draw_script(object);
      break;
  }

}


