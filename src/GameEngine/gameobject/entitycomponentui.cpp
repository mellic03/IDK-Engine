#include "entitycomponent.h"


bool EntityComponentUI::newComponent(EntityComponentType *component_type)
{
  if (ImGui::BeginPopupContextWindow("New Component"))
  {
    ImGui::Text("New Component");
    ImGui::Separator();

    if (ImGui::MenuItem("Transform"))
      *component_type = COMPONENT_TRANSFORM;

    if (ImGui::MenuItem("Script"))
      *component_type = COMPONENT_SCRIPT;

    ImGui::EndPopup();
  }

  return *component_type != COMPONENT_NONE;
}
