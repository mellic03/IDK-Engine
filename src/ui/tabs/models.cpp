#include "../UIEngine.h"




void EngineUI::gameobjects()
{
  ImGui::Begin("gameobjects");

  ImGui::Text("gameobjects");
  ImGui::Separator();

  std::list<GameObject> *gameobjects = Scene::scenegraph.getTemplates();

  std::string label = "";
  label += EngineUI::getObjectIcon(GAMEOBJECT_TERRAIN);
  label += "  Terrain";

  if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    for (GameObject *obj: *Scene::scenegraph.getTemplatesByType(GAMEOBJECT_TERRAIN))
    {
      ImGui::Text(obj->getTemplateName().c_str());
    }

    ImGui::TreePop();
  }


  ImGui::End();
}


