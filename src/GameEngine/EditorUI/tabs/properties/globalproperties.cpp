#include "../../UIEngine.h"
#include "entitycomponentui.h"



static void globalLOD(GameObject *object)
{
  ModelLOD *modelLOD = object->getModelLOD();
  if (modelLOD == nullptr)
    return;

  std::string label = "";
  label += ICON_FA_EYE;
  label += " " + object->getTemplateName();


  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    const int max_lod = modelLOD->getLevelsLOD() - 1;
  
    ImGui::SliderInt("LOD", modelLOD->getMaxLOD_value(), 0, max_lod);
    ImGui::SliderInt("Shadow LOD", modelLOD->getShadowLOD_value(), 0, max_lod);
  
    std::vector<float> lod_distances = *modelLOD->getLODDistances();
    for (int i=0; i<max_lod; i++)
    {
      ImGui::Text("%d: %f\n", i, lod_distances[i]);
    }
  }
}


static void global_do_thing(GameObject *object)
{

  std::list<GameObject *> *objects = Scene::scenegraph.getInstancesByTemplateName(object->getTemplateName());
  
  for (GameObject *object: *objects)
  {
    ImGui::Text("%s", object->getName().c_str());
  }

}


void EngineUI::globalProperties()
{
  ImGui::Begin("Global Properties");

  ImGui::Text("Global Properties");
  ImGui::Separator();


  SceneGraph *scenegraph = &Scene::scenegraph;
  if (scenegraph->m_object_instances.size() == 0)
    return;

  GameObject *object = scenegraph->objectPtr(EngineUI::selected_objectID);
  if (object == nullptr)
    return;


  globalLOD(object);

  global_do_thing(object);


  ImGui::End();
}

