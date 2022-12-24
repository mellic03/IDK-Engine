#include "../ui.h"



void EngineUI::properties(Scene *scene)
{
  ImGui::Begin("Properties");
  {
    SceneGraph *scenegraph = scene->m_scenegraph;

    if (scenegraph->m_object_instances.size() == 0)
      return;


    GameObject *object = scenegraph->objectPtr(EngineUI::selected_objectID);

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

    


    ImGui::Text("Other Stuff");
    ImGui::Separator();
    ImGui::Checkbox("Hidden", object->isHiddenPtr());

    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::Text("Info");
    ImGui::Separator();

    ImGui::Text("environmental:       %s", (object->isEnvironmental() ? "true" : "false"));
    ImGui::Text("physics_state:       %s", object->physStateString().c_str());
    ImGui::Text("navigation_state:    %s", object->navStateString().c_str());
    ImGui::Text("has collision mesh:  %s", object->hasCollisionMesh() ? "true": "false");


  }
  ImGui::End();
}