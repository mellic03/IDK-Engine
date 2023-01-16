#include "../../UIEngine.h"

bool EntityComponentUI::newComponent(EntityComponentType *component_type)
{
  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("New");

  if (ImGui::BeginPopup("New"))
  {
    if (ImGui::MenuItem("Transform"))
      *component_type = COMPONENT_TRANSFORM;

    if (ImGui::MenuItem("Script"))
      *component_type = COMPONENT_SCRIPT;

    ImGui::EndPopup();
  }

  return *component_type != COMPONENT_NONE;
}
