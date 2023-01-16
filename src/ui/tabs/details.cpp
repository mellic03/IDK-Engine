#include "../UIEngine.h"


void EngineUI::details(Scene *scene)
{
  GameObject *object = scene->m_scenegraph->objectPtr(EngineUI::selected_objectID);
  if (object == nullptr)
    return;
    
  ImGui::Begin("Details");
  {
    ImGui::Text("Internal type: %s", object->getTemplateName().c_str());
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Checkbox("Hidden", object->isHiddenPtr());
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Info");
    ImGui::Separator();
    ImGui::Text("GameObjectType:      %s", object->getObjectTypeString().c_str());
    ImGui::Text("environmental:       %s", (object->isEnvironmental() ? "true" : "false"));
    ImGui::Text("physics_state:       %s", object->physStateString().c_str());
    ImGui::Text("navigation_state:    %s", object->navStateString().c_str());
    ImGui::Text("has collision mesh:  %s", object->hasCollisionMesh() ? "true": "false");
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Scripting Access");
    ImGui::Separator();
    ImGui::Text("Lua objectID: %d", object->getID() + 1);

    ImGui::End();
  }

}