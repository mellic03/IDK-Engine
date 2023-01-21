#include "../UIEngine.h"



void EngineUI::debug(Renderer *ren)
{
  ImGui::Begin("Debug");

  ImGui::Text("Debug");
  ImGui::Separator();


  EngineUI::bitFlagCheckBox(
    "Draw bounding spheres",
    static_cast<unsigned char>(RenderDebugFlag::DrawBoundingSpheres),
    (unsigned char *)(ren->getDebugData()->getDebugFlags())
  );

  ImGui::End();
}

