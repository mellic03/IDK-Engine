#include "../UIEngine.h"


void EngineUI::details()
{
  GameObject *object = Scene::scenegraph.objectPtr(EngineUI::selected_objectID);
  if (object == nullptr)
    return;
    
  ImGui::Begin("Details");
  {
    GameObjectData *data = object->getData();

    ImGui::Text("Internal type: %s", object->getTemplateName().c_str());
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Info");
    ImGui::Separator();
    ImGui::Text("GameObjectType:      %s", GameObjectUtil::ToString::gameObjectType(object->getObjectType()).c_str());
    ImGui::Text("GameObjectFlags:     %u", *data->flags()->bits());
    if (ImGui::Button("thing"))
    {
      data->flags()->set(GameObjectFlag::GEOMETRY, true);
    }
    ImGui::Text("Animated:            %d", data->flags()->get(GameObjectFlag::ANIMATED));
    ImGui::Text("sphere:              %d", object->getComponents()->hasComponent(COMPONENT_SPHERE_COLLIDER));
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("Scripting Access");
    ImGui::Separator();
    ImGui::Text("Lua objectID: %d", object->getID() + 1);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));


    ImGui::End();
  }
}

