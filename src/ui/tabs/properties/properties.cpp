#include "../../UIEngine.h"
#include "entitycomponentui.h"



void EngineUI::instanceProperties()
{
  SceneGraph *scenegraph = &Scene::scenegraph;
  if (scenegraph->m_object_instances.size() == 0)
    return;

  GameObject *object = scenegraph->objectPtr(EngineUI::selected_objectID);
  if (object == nullptr)
    return;

  
  ImGui::Begin("Instance Properties");
  {
    // ImGui::PushID(EngineUI::selected_objectID);
    // EngineUI::dragVec3("emission", &object->emission, 0.1f);
    // ImGui::DragFloat("scale", &object->emission_scale, 0.01f, 0.0f, 10.0f);
    // ImGui::PopID();

    bool icon_changed = false;
    std::string title = EngineUI::getObjectIcon(object->getData()->ui_icon_type);

    if (ImGui::Button(title.c_str()))
      ImGui::OpenPopup("change icon");

    if (ImGui::BeginPopup("change icon"))
    {
      for (int i=0; i<(int)GAMEOBJECT_NUM_TYPES; i++)
      {
        if (ImGui::MenuItem(EngineUI::getObjectIcon((GameObjectType)i).c_str()))
          object->getData()->setUiIconType((GameObjectType)i);
      }
      ImGui::EndPopup();
    }

    ImGui::SameLine();

    std::string label = "##" + std::to_string(object->getID());
    ImGui::InputText(label.c_str(), &object->m_given_name);


    ImGui::Separator();

    int count = 0;

    EntityComponents *components = object->getComponents();

    for (int i=0; i<COMPONENT_NUM_COMPONENTS; i++)
    {
      if (components->hasComponent((EntityComponentType)i))
        EntityComponentUI::drawComponent(object, components, (EntityComponentType)i);
    }


    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    EntityComponentUI::newComponent(object);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

  }
  ImGui::End();
}