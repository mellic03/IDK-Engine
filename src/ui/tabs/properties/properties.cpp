#include "../../UIEngine.h"

void EngineUI::properties(Scene *scene)
{
  SceneGraph *scenegraph = scene->m_scenegraph;
  if (scenegraph->m_object_instances.size() == 0)
    return;

  GameObject *object = scenegraph->objectPtr(EngineUI::selected_objectID);
  if (object == nullptr)
    return;

  
  ImGui::Begin("Properties");
  {
    ImGui::PushID(EngineUI::selected_objectID);
    EngineUI::vec3("emission", &object->emission, 0.1f);
    ImGui::DragFloat("scale", &object->emission_scale, 0.01f, 0.0f, 10.0f);
    ImGui::PopID();


    ImGui::InputText("Name", &object->m_given_name);
    ImGui::Separator();

    int count = 0;

    for (auto &component: object->transform_components)
    {
      ImGui::PushID(count);
      component.draw(object);
      ImGui::PopID();
      count += 1;
    }

    for (auto &component: object->lightsource_components)
    {
      ImGui::PushID(count);
      component.draw(object);
      ImGui::PopID();
      count += 1;
    }

    for (auto &component: object->script_components)
    {
      ImGui::PushID(count);
      component.draw(object);
      ImGui::PopID();
      count += 1;
    }

    for (auto &component: object->variable_components)
    {
      ImGui::PushID(count);
      component.draw(object);
      ImGui::PopID();
      count += 1;
    }

    for (auto &component: object->terrain_components)
    {
      ImGui::PushID(count);
      component.draw(object);
      ImGui::PopID();
      count += 1;
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    EntityComponentType component_type = COMPONENT_NONE;
    if (EntityComponentUI::newComponent(&component_type))
    {
      switch (component_type)
      {
        case (COMPONENT_TRANSFORM):
          object->transform_components.push_back(EntityComponent(component_type));
          break;

        case (COMPONENT_SCRIPT):
          object->script_components.push_back(EntityComponent(component_type));
          break;
      }
    }
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

  }
  ImGui::End();
}