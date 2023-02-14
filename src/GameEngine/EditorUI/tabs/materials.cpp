#include "../UIEngine.h"



void EngineUI::materials(GameObject *object)
{

  ImGui::Begin("Materials");

  Model *model = object->getModel();

  if (model != nullptr)
  {
    for (Material &material: model->mesh.materials)
    {
      ImGui::Text("material: %s", material.diffuseMap.m_dae_id);
    }

  }


  ImGui::End();
}


