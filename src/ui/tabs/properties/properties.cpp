#include "../../UIEngine.h"
#include "entitycomponentui.h"

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
    // ImGui::PushID(EngineUI::selected_objectID);
    // EngineUI::dragVec3("emission", &object->emission, 0.1f);
    // ImGui::DragFloat("scale", &object->emission_scale, 0.01f, 0.0f, 10.0f);
    // ImGui::PopID();


    ImGui::InputText("Name", &object->m_given_name);
    ImGui::Separator();

    int count = 0;

    for (int i=0; i<COMPONENT_NUM_COMPONENTS; i++)
    {
      if (object->entity_components.hasComponent((EntityComponentType)i))
        EntityComponentUI::drawComponent(object, object->entity_components.getComponent((EntityComponentType)i));
    }


    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    EntityComponentType component_type = COMPONENT_NONE;

    if (EntityComponentUI::newComponent(&component_type))
      object->entity_components.giveComponent(component_type);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

  }
  ImGui::End();
}