#include "../ui.h"



void EngineUI::render(Renderer *ren)
{
  ImGui::Begin("Render");

  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  if (ImGui::Button("Recompile Shaders"))
  {
    ren->compileShaders();
  }

  ImGui::ColorEdit3("Sky colour", (float*)&ren->clearColor);
  ImGui::ColorEdit3("Horizon colour", (float*)&ren->horizonColor);
  ImGui::DragFloat("start", &ren->transition_start, 0.01f);
  ImGui::DragFloat("stop", &ren->transition_stop, 0.01f);

  ImGui::SliderFloat("Fog start", &ren->fog_start, 0.0f, 100.0f, "%0.1f", 0);
  ImGui::SliderFloat("Fog end",   &ren->fog_end,   0.0f, 1000.0f, "%0.1f", 0);

  ImGui::SliderFloat("near plane",  &ren->near_plane,  0.1f, 1.0f, "%0.1f", 0);
  ImGui::SliderFloat("Far plane",   &ren->far_plane,   1.0f, 1000.0f, "%0.1f", 0);


  ImGui::Dummy(ImVec2(0.0f, 20.0f));
  ImGui::Text("Shadow Cascades");
  ImGui::Separator();
  
  ImGui::DragFloat("Cascade 0", &ren->shadow_cascades[0], 1.0f);
  float f = 0.0f;
  ImGui::DragFloat("Bias #0", &f);
  ImGui::Dummy(ImVec2(0.0f, 5.0f));

  ImGui::DragFloat("Cascade 1", &ren->shadow_cascades[1], 1.0f);
  ImGui::DragFloat("Cascade 2", &ren->shadow_cascades[2], 1.0f);
  ImGui::DragFloat("Cascade 3", &ren->shadow_cascades[3], 1.0f);
  ImGui::DragFloat("Cascade 4", &ren->shadow_cascades[4], 1.0f);
  ImGui::DragFloat("Cascade 5", &ren->shadow_cascades[5], 1.0f);
  ImGui::DragFloat("Cascade 6", &ren->shadow_cascades[6], 1.0f);
  ImGui::DragFloat("Cascade 7", &ren->shadow_cascades[7], 1.0f);


  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  ImGui::SliderFloat("FOV", &ren->fov, 45.0f, 110.0f);
  ImGui::SliderFloat("Exposure", &ren->exposure, 0.0f, 10.0f);
  ImGui::SliderFloat("Gamma", &ren->gamma, 0.0f, 5.0f);

  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  ImGui::Text("Image Kernel");

  if (ImGui::BeginTable("Image Kernel", 3))
  {
    char buffer[32];

    for (int row=0; row<3; row++)
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::PushID(3*row+0);
      ImGui::DragScalar("", ImGuiDataType_Float, &ren->image_kernel[3*row+0], 1.0f, 0);
      ImGui::PopID();
      
      ImGui::TableNextColumn();
      ImGui::PushID(3*row+1);
      ImGui::DragScalar("", ImGuiDataType_Float, &ren->image_kernel[3*row+1], 1.0f, 0);
      ImGui::PopID();

      ImGui::TableNextColumn();
      ImGui::PushID(3*row+2);
      ImGui::DragScalar("", ImGuiDataType_Float, &ren->image_kernel[3*row+2], 1.0f, 0);
      ImGui::PopID();
    }
    ImGui::EndTable();
  }

  if (ImGui::Button("Invert"))
  {
    for (int i=0; i<9; i++)
      ren->image_kernel[i] = -ren->image_kernel[i];
  }
  if (ImGui::Button("Reset"))
  {
    for (int i=0; i<9; i++)
      ren->image_kernel[i] = 0.0f;
    ren->image_kernel[4] = 1.0f;
  }

  ImGui::DragScalar("Divisor", ImGuiDataType_Float, &ren->kernel_divisor, 1.0f, NULL);
  ImGui::DragScalar("Pixel offset divisor", ImGuiDataType_Float, &ren->kernel_offset_divisor, 1.0f, NULL);


  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


  ImGui::End();
}
