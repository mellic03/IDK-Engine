#include "../../UIEngine.h"
#include "entitycomponentui.h"
#include "drawecs.h"


void EntityComponentUI::newComponent(GameObject *object)
{
  const std::string id = " ##" + std::to_string(object->getID());

  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("New");

  EntityComponents *components = object->getComponents();

  if (ImGui::BeginPopup("New"))
  {
    if (ImGui::MenuItem(EngineUI::IconLabels::TRANSFORM_LABEL + id))
      components->giveComponent(TransformComponent());

    else if (ImGui::MenuItem(EngineUI::IconLabels::SCRIPT_LABEL + id))
      components->giveComponent(ScriptComponent("LuaScripting/scripts/default"));

    else if (ImGui::MenuItem(EngineUI::IconLabels::LOD_LABEL + id))
      components->giveComponent(LODComponent());

    else if (ImGui::MenuItem(EngineUI::IconLabels::SPHERE_LABEL + id))
      components->giveComponent(COMPONENT_SPHERE_COLLIDER);

    else if (ImGui::MenuItem(EngineUI::IconLabels::CAPSULE_LABEL + id))
      components->giveComponent(&object->capsulecollider);

    else if (ImGui::MenuItem(EngineUI::IconLabels::MESH_LABEL + id))
      components->giveComponent(MeshColliderComponent());

    else if (ImGui::MenuItem(EngineUI::IconLabels::PHYSICS_LABEL + id))
      components->giveComponent(COMPONENT_PHYSICS);
  
    else if (ImGui::MenuItem(EngineUI::IconLabels::NAVIGATION_LABEL + id))
      components->giveComponent(COMPONENT_NAVIGATION);

    else if (ImGui::MenuItem("bounding box" + id))
      components->giveComponent(COMPONENT_BOUNDING_BOX);

    ImGui::EndPopup();
  }
}


void EntityComponentUI::drawComponent(GameObject *object, EntityComponents *components, EntityComponentType component_type)
{
  switch (component_type)
  {
    case (COMPONENT_NONE):
      break;


    case (COMPONENT_TRANSFORM):
      DrawECS::transform(object, components->getTransformComponent());
      break;


    case (COMPONENT_POINT_LIGHT):
      DrawECS::pointlight(object, components->getPointLightComponent());
      break;


    case (COMPONENT_SCRIPT):
      {
        std::vector<ScriptComponent> *script_components = components->getScriptComponents();
        
        int i = 0;
        for (ScriptComponent &script_component: *script_components)
        {
          ImGui::PushID(i);
          DrawECS::script(object, &script_component);
          ImGui::PopID();
          i += 1;
        }
      }
      break;
  

    case (COMPONENT_TERRAIN):
      DrawECS::terrain(object, components->getTerrainComponent());
      break;
    
    
    case (COMPONENT_LOD):
      DrawECS::lod(object, components->getLODComponent());
      break;

  
    case (COMPONENT_SPHERE_COLLIDER):
      DrawECS::sphere(object, components->getSphereColliderComponent());
      break;


    case (COMPONENT_CAPSULE_COLLIDER):
      DrawECS::capsule(object, components->getCapsuleColliderComponent());
      break;
  

    case (COMPONENT_ANIMATION):
      DrawECS::animation(object);
      break;
      

    case (COMPONENT_PHYSICS):
      DrawECS::physics(object);
      break;
  

    case (COMPONENT_NAVIGATION):
      DrawECS::navigation(object);
      break;


    case (COMPONENT_BOUNDING_BOX):
      DrawECS::boundingbox(object);
      break;
  }
}