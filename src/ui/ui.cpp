#include <stdio.h>

#include "UIEngine.h"

#include "ui.h"

#include <fstream>
#include <sstream>

int selected_dirlight = 0;    const char *dir_options[4] = {"1"};
int selected_pointlight = 0;  const char *point_options[4] = {"1", "2", "3", "4"};
int selected_spotlight = 0;   const char *spot_options[4] = {"1", "2"};


void draw_lighting_tab(Renderer *ren, Scene *scene)
{
  ImGui::Text("Directional Light");
  ImGui::Separator();
  ImGui::DragFloat3("Position", &scene->m_scenegraph->dirlight.position[0], 0.1f);
  ImGui::ColorEdit3("Diffuse", &scene->m_scenegraph->dirlight.diffuse[0], 0.1f);
  ImGui::ColorEdit3("Ambient", &scene->m_scenegraph->dirlight.ambient[0], 0.1f);
  ImGui::DragFloat("Fog intensity", &scene->m_scenegraph->dirlight.fog_intensity, 0.01f);    
  ImGui::Dummy(ImVec2(0.0f, 20.0f));


  ImGui::Text("Volumetrics");
  ImGui::Separator();
  ImGui::SliderInt("Resolution",  &ren->volumetrics.resolution_divisor, 1, 64);
  ImGui::DragInt("Blur passes ##volumetric", &ren->volumetrics.num_blur_passes, 1.0f, 0, 64);
  ImGui::DragFloat("Texel size ##volumetric", &ren->volumetrics.texel_size, 0.01f, 0, 256);
  ImGui::DragFloat("Horizontal strength ##volumetric", &ren->volumetrics.x_strength, 0.01f, 0, 256);
  ImGui::DragFloat("Vertical strength ##volumetric",   &ren->volumetrics.y_strength, 0.01f, 0, 256);

  ImGui::DragInt("Samples",      &ren->volumetrics.num_samples);
  ImGui::DragFloat("Step size",    &ren->volumetrics.step_size, 0.001f);
  ImGui::DragFloat("Step multiplier",    &ren->volumetrics.step_multiplier, 0.001f);

  ImGui::Text("Volumetric pass time: %lf\n", ren->render_pass_timer.getVolumetric());
  ImGui::Dummy(ImVec2(0.0f, 20.0f));



  ImGui::Text("Bloom");
  ImGui::Separator();
  ImGui::DragInt("Blur passes ##bloom",           &ren->bloomData.num_blur_passes, 1.0f, 0, 256);
  ImGui::DragFloat("Texel size ##bloom",          &ren->bloomData.texel_size, 0.01f, 0, 256);
  ImGui::DragFloat("Horizontal strength ##bloom", &ren->bloomData.x_strength, 0.01f, 0, 256);
  ImGui::DragFloat("Vertical strength ##bloom",   &ren->bloomData.y_strength, 0.01f, 0, 256);

}

void draw_render_tab(Renderer *ren)
{
  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  if (ImGui::Button("Recompile Shaders"))
  {
    ren->compileShaders();
  }

  ImGui::ColorEdit3("Clear colour", (float*)&ren->clearColor);
  ImGui::SliderFloat("Fog start", &ren->fog_start, 0.0f, 100.0f, "%0.1f", 0);
  ImGui::SliderFloat("Fog end",   &ren->fog_end,   0.0f, 1000.0f, "%0.1f", 0);

  ImGui::SliderFloat("near plane",  &ren->near_plane,  0.1f, 1.0f, "%0.1f", 0);
  ImGui::SliderFloat("Far plane",   &ren->far_plane,   1.0f, 1000.0f, "%0.1f", 0);


  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  ImGui::SliderFloat("FOV", &ren->fov, 45.0f, 110.0f);
  ImGui::SliderFloat("Exposure", &ren->exposure, 0.0f, 10.0f);
  ImGui::SliderFloat("Gamma", &ren->gamma, 0.0f, 5.0f);

  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  ImGui::Text("Image Kernel");

  if (ImGui::BeginTable("Image Kernel", 3))
  {
    char buffer[32];

    for (int row = 0; row < 3; row++)
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
}

void draw_physics_tab(Renderer *ren, Player *player)
{
  ImGui::SliderFloat("Gravity", &ren->gravity, 0, 10);
  ImGui::Checkbox("Fly", &player->fly);
}


void draw_dev_ui(Renderer *ren, Scene *scene, Player *player, int *x, int *y, int *w, int *h)
{
  draw_main_menu_bar(ren, scene);

  ImGuiWindowFlags windowflags = 0;
  windowflags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  windowflags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing;
  windowflags |= ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_MenuBar;
  windowflags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

  ImGui::Begin("Root", NULL, windowflags);
  {
    ImGui::DockSpace(ImGui::GetID("dock"));

    EngineUI::sceneHierarchy(ren, scene);
    EngineUI::properties(scene);
    EngineUI::scriptBrowser();
    EngineUI::scriptEditor();
    EngineUI::details(scene);

    ImGui::Begin("Depth Map");
    {
      ImVec2 viewportsize = ImGui::GetContentRegionAvail();
      
      ImGui::BeginGroup();
      ImGui::Text("position");
      ImGui::Image((ImTextureID)ren->gbuffer_position, {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();

      ImGui::SameLine();

      ImGui::BeginGroup();
      ImGui::Text("normal");
      ImGui::Image((ImTextureID)ren->gbuffer_normal, {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();

      ImGui::SameLine();

      ImGui::BeginGroup();
      ImGui::Text("albedo");
      ImGui::Image((ImTextureID)ren->gbuffer_albedospec, {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();



      ImGui::BeginGroup();
      ImGui::Text("bloom");
      ImGui::Image((ImTextureID)ren->colorBuffers[1], {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();

      ImGui::SameLine();
      
      ImGui::BeginGroup();
      ImGui::Text("volumetrics");
      ImGui::Image((ImTextureID)ren->lightshaftColorBuffer, {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();

      ImGui::SameLine();

      ImGui::BeginGroup();
      ImGui::Text("general");
      ImGui::Image((ImTextureID)ren->generalColorBuffer, {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();


      ImGui::BeginGroup();
      ImGui::Text("Dirlight depthmap");
      ImGui::Image((ImTextureID)ren->dirlight_depthmap, {viewportsize.x/3, viewportsize.y/3}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();

      ImGui::End();
    }

    ImGui::Begin("Lighting");
    {
      draw_lighting_tab(ren, scene);
      ImGui::End();
    }
  
    ImGui::Begin("Render");
    {
      draw_render_tab(ren);
      ImGui::End();
    }

    ImGui::Begin("Physics");
    {
      draw_physics_tab(ren, player);
      ImGui::End();
    }

    ImGui::Begin("Viewport");
    {
      *w = (int)ImGui::GetContentRegionAvail().x;
      *h = (int)ImGui::GetContentRegionAvail().y;

      if (ren->viewport_width != *w || ren->viewport_height != *h)
        ren->resize(*w, *h);

      ImGui::Image((ImTextureID)ren->screenQuadColorBuffers[0], ImGui::GetContentRegionAvail(), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  
      ImGui::End();
    }


    ImGui::End();
  }
}