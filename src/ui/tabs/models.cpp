#include "../UIEngine.h"




void EngineUI::models()
{
  ImGui::Begin("Models");

  ImGui::Text("Models");
  ImGui::Separator();

  std::list<GameObject> *gameobjects = Scene::scenegraph.getTemplates();

  if (ImGui::TreeNodeEx("Terrain", ImGuiTreeNodeFlags_DefaultOpen))
  {

    ImGui::TreePop();
  }



  ImGui::End();
}


