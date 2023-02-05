#include "../UIEngine.h"

void EngineUI::physics()
{
  ImGui::Begin("Physics");
  {
    ImGui::SliderFloat("Gravity", &PhysicsEngine::gravity, 0, 10);
    ImGui::End();
  }
}
