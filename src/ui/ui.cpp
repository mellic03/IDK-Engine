#include <stdio.h>

#include "UIEngine.h"

#include "ui.h"

#include <fstream>
#include <sstream>

int selected_dirlight = 0;    const char *dir_options[4] = {"1"};
int selected_pointlight = 0;  const char *point_options[4] = {"1", "2", "3", "4"};
int selected_spotlight = 0;   const char *spot_options[4] = {"1", "2"};


void export_lighting_config(Renderer *ren)
{
  FILE *fh = fopen("config.txt", "w");
  
  for (int i=0; i<NUM_DIRLIGHTS; i++)
  {
    DirLight *light = &ren->dirlights[i];
    fprintf(fh, "DIRLIGHT AMBIENT: %f %f %f\n", light->ambient.x, light->ambient.y, light->ambient.z);
    fprintf(fh, "DIRLIGHT DIFFUSE: %f %f %f\n", light->diffuse.x, light->diffuse.y, light->diffuse.z);
    fprintf(fh, "DIRLIGHT DIRECTION: %f %f %f\n", light->direction.x, light->direction.y, light->direction.z);
    fprintf(fh, "DIRLIGHT BIAS: %f\n", ren->DIRBIAS);
  }

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    PointLight *light = &ren->pointlights[i];
    fprintf(fh, "POINTLIGHT ENABLED: %d\n", ren->pointlights_on[i]);
    fprintf(fh, "POINTLIGHT AMBIENT: %f %f %f\n", light->ambient.x, light->ambient.y, light->ambient.z);
    fprintf(fh, "POINTLIGHT DIFFUSE: %f %f %f\n", light->diffuse.x, light->diffuse.y, light->diffuse.z);
    fprintf(fh, "POINTLIGHT CONSTANT, LINEAR, QUADRATIC: %f %f %f\n", light->constant, light->linear, light->quadratic);
    // fprintf(fh, "POINTLIGHT POSITION: %f %f %f\n", light->position.x, light->position.y, light->position.z);
  }

  fprintf(fh, "CLEAR COLOR: %f %f %f\n", ren->clearColor.x, ren->clearColor.y, ren->clearColor.z);
  fprintf(fh, "FOG START, STOP: %f %f\n", ren->fog_start, ren->fog_end);
  fprintf(fh, "FOV: %f\n", ren->fov);
  fprintf(fh, "EXPOSURE: %f\n", ren->exposure);
  fprintf(fh, "GAMMA: %f\n", ren->gamma);


  fclose(fh);
}

void import_lighting_config(Renderer *ren)
{
  FILE *fh = fopen("config.txt", "r");
  
  for (int i=0; i<NUM_DIRLIGHTS; i++)
  {
    DirLight *light = &ren->dirlights[i];
    fscanf(fh, "DIRLIGHT AMBIENT: %f %f %f\n", &light->ambient.x, &light->ambient.y, &light->ambient.z);
    fscanf(fh, "DIRLIGHT DIFFUSE: %f %f %f\n", &light->diffuse.x, &light->diffuse.y, &light->diffuse.z);
    fscanf(fh, "DIRLIGHT DIRECTION: %f %f %f\n", &light->direction.x, &light->direction.y, &light->direction.z);
    fscanf(fh, "DIRLIGHT BIAS: %f\n", &ren->DIRBIAS);
  }

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    PointLight *light = &ren->pointlights[i];
    fscanf(fh, "POINTLIGHT ENABLED: %d\n", &ren->pointlights_on[i]);
    fscanf(fh, "POINTLIGHT AMBIENT: %f %f %f\n", &light->ambient.x, &light->ambient.y, &light->ambient.z);
    fscanf(fh, "POINTLIGHT DIFFUSE: %f %f %f\n", &light->diffuse.x, &light->diffuse.y, &light->diffuse.z);
    fscanf(fh, "POINTLIGHT CONSTANT, LINEAR, QUADRATIC: %f %f %f\n", &light->constant, &light->linear, &light->quadratic);
    // fscanf(fh, "POINTLIGHT POSITION: %f %f %f\n", &light->position.x, &light->position.y, &light->position.z);
  }

  fscanf(fh, "CLEAR COLOR: %f %f %f\n", &ren->clearColor.x, &ren->clearColor.y, &ren->clearColor.z);
  fscanf(fh, "FOG START, STOP: %f %f\n", &ren->fog_start, &ren->fog_end);
  fscanf(fh, "FOV: %f\n", &ren->fov);
  fscanf(fh, "EXPOSURE: %f\n", &ren->exposure);
  fscanf(fh, "GAMMA: %f\n", &ren->gamma);

  fclose(fh);
}


void draw_lighting_tab(Renderer *ren)
{
  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  char buffer[64];

  if (ImGui::TreeNode("Directional Lights"))
  {
    ImGui::ListBox("Directional light #", &selected_dirlight, dir_options, 1, -1);

    ImGui::PushID(1);
    ImGui::ColorEdit3("ambient", (float*)&ren->dirlights[selected_dirlight].ambient);
    ImGui::ColorEdit3("diffuse", (float*)&ren->dirlights[selected_dirlight].diffuse);
    
    ImGui::Text("Direction");
    ImGui::DragScalar("x", ImGuiDataType_Float, &ren->dirlights[selected_dirlight].direction.x, 0.05f, NULL);
    ImGui::DragScalar("y", ImGuiDataType_Float, &ren->dirlights[selected_dirlight].direction.y, 0.05f, NULL);
    ImGui::DragScalar("z", ImGuiDataType_Float, &ren->dirlights[selected_dirlight].direction.z, 0.05f, NULL);
    // ren->dirlights[0].direction = glm::normalize(ren->dirlights[0].direction);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::DragScalar("bias", ImGuiDataType_Float, &ren->DIRBIAS, 0.005f, NULL);
    ImGui::PopID();
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Point Lights"))
  {
    ImGui::ListBox("Point light #", &selected_pointlight, point_options, 4, -1);

    ImGui::PushID(1);
    
    ImGui::Checkbox("Enable", &ren->pointlights_on[selected_pointlight]);

    ImGui::ColorEdit3("ambient", (float*)&ren->pointlights[selected_pointlight].ambient);
    ImGui::ColorEdit3("diffuse", (float*)&ren->pointlights[selected_pointlight].diffuse);
    
    ImGui::SliderFloat("constant", &ren->pointlights[selected_pointlight].constant, 0.0f, 100.0f, "%0.4f", 0);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].quadratic, 0.001f, 0);
    ImGui::DragScalar("bias", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].bias, 0.001f, 0);

    ImGui::Text("Position");
    // ImGui::DragScalar("x", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.x, 0.05f, 0);
    // ImGui::DragScalar("y", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.y, 0.05f, 0);
    // ImGui::DragScalar("z", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.z, 0.05f, 0);
    ImGui::PopID();
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Spot Lights"))
  {
    ImGui::ListBox("Point light #", &selected_spotlight, spot_options, 2, -1);

    ImGui::Checkbox("Enable", &ren->spotlights_on[selected_spotlight]);

    ImGui::PushID(3);
    ImGui::ColorEdit3("ambient", (float*)&ren->spotlights[selected_spotlight].ambient);
    ImGui::ColorEdit3("diffuse", (float*)&ren->spotlights[selected_spotlight].diffuse);

    // ImGui::DragScalar("constant", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].constant,   0.001f, NULL);
    ImGui::SliderFloat("constant", &ren->spotlights[selected_spotlight].constant, 0.0f, 100.0f, "%0.4f", 0);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].quadratic, 0.001f, 0);

    ImGui::DragScalar("intensity", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].intensity, 0.05f, 0);
    ImGui::DragScalar("inner cutoff", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].inner_cutoff, 0.05f, 0);
    ImGui::DragScalar("outer cutoff", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].outer_cutoff, 0.05f, 0);
    ImGui::DragScalar("fov", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].fov, 0.05f, 0);

    ImGui::Checkbox("follow", &ren->spotlights[selected_spotlight].follow);



    ImGui::PushID(4);
    ImGui::Text("Position");
    ImGui::DragScalar("x", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].position.x, 0.05f, 0);
    ImGui::DragScalar("y", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].position.y, 0.05f, 0);
    ImGui::DragScalar("z", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].position.z, 0.05f, 0);
    ImGui::PopID();
    
    ImGui::PushID(5);
    ImGui::Text("Direction");
    ImGui::DragScalar("x", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].direction.x, 0.05f, 0);
    ImGui::DragScalar("y", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].direction.y, 0.05f, 0);
    ImGui::DragScalar("z", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].direction.z, 0.05f, 0);
    ren->spotlights[selected_spotlight].direction = glm::normalize(ren->spotlights[selected_spotlight].direction);
    ImGui::PopID();
    ImGui::PopID();
    ImGui::TreePop();
  }

  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  if (ImGui::Button("Import"))
    import_lighting_config(ren);
  ImGui::SameLine(ImGui::GetWindowWidth()-60);
  if (ImGui::Button("Export"))
    export_lighting_config(ren);


}

void draw_render_tab(Renderer *ren)
{
  ImGui::Dummy(ImVec2(0.0f, 20.0f));

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

void draw_physics_tab(Renderer *ren)
{
  ImGui::SliderFloat("Gravity", &ren->gravity, 0, 10);
  bool fly = false;
  ImGui::Checkbox("Fly", &fly);
}

char script_buffer[2048];
bool first = true;

void draw_dev_ui(Renderer *ren, Scene *scene, int *x, int *y, int *w, int *h)
{
  draw_main_menu_bar(ren, scene);

  ImGuiInputTextFlags scriptflags = 0;
  scriptflags |= ImGuiInputTextFlags_AllowTabInput;

  ImGuiWindowFlags windowflags = 0;
  windowflags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  windowflags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing;
  windowflags |= ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_MenuBar;
  windowflags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

  if (first)
  {
    std::ifstream fh;
    fh.open("LuaScripting/scripts/default.lua");

    std::string raw_file = "";
    std::string line;
    while (getline(fh, line))
      raw_file += line + "\n";

    strcpy(script_buffer, raw_file.c_str());

    first = false;
  }

  ImGui::Begin("Root", NULL, windowflags);
  {
    ImGui::DockSpace(ImGui::GetID("dock"));


    EngineUI::sceneHierarchy(ren, scene);
    EngineUI::properties(scene);
    EngineUI::scriptEditor();



    ImGui::Begin("Lighting");
    {
      draw_lighting_tab(ren);
      ImGui::End();
    }
  
    ImGui::Begin("Render");
    {
      draw_render_tab(ren);
      ImGui::End();
    }

    ImGui::Begin("Physics");
    {
      draw_physics_tab(ren);
      ImGui::End();
    }

    static std::filesystem::path selected_path("");


    ImGui::Begin("Script editor");
    {
      ImGui::InputTextMultiline("Script", script_buffer, 2048, ImVec2(-1, ImGui::GetContentRegionAvail().y-35.0f), scriptflags);
      
      if (ImGui::Button("Save"))
      {
        std::ofstream fh;
        fh.open(selected_path.string(), std::ios::out);
        fh << script_buffer;
        fh.close();
      }

      ImGui::End();
    }


    ImGui::Begin("Viewport");
    {
      *w = (int)ImGui::GetContentRegionAvail().x;
      *h = (int)ImGui::GetContentRegionAvail().y;

      ImVec2 viewportsize = ImGui::GetContentRegionAvail();

      if (ren->viewport_width != *w || ren->viewport_height != *h)
        ren->resize(*w, *h);

      ImGui::Image((ImTextureID)ren->screenColorBuffers[0], ImGui::GetContentRegionAvail(), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  
      ImGui::End();
    }


    ImGui::End();
  }
}