#include "ui.h"

int selected_dirlight = 0;
int selected_pointlight = 0;
int selected_spotlight = 0;
bool show = false;


void draw_lighting_tab(Renderer *ren)
{

  ImGui::Dummy(ImVec2(0.0f, 20.0f));


  char buffer[64];

  if (ImGui::TreeNode("Directional Lights"))
  {
    for (int n = 0; n < NUM_DIRLIGHTS; n++)
    {
      sprintf(buffer, "directional light %d", n);
      if (ImGui::Selectable(buffer, selected_dirlight == n))
          selected_dirlight = n;
    }
    ImGui::TreePop();
  }
  ImGui::PushID(1);
  ImGui::ColorEdit3("ambient", (float*)&ren->dirlights[selected_dirlight].ambient);
  ImGui::ColorEdit3("diffuse", (float*)&ren->dirlights[selected_dirlight].diffuse);
  ImGui::ColorEdit3("specular", (float*)&ren->dirlights[selected_dirlight].specular);
  
  ImGui::Text("Direction");
  ImGui::DragScalar("x", ImGuiDataType_Float, &ren->dirlights[selected_dirlight].direction.x, 0.05f, NULL);
  ImGui::DragScalar("y", ImGuiDataType_Float, &ren->dirlights[selected_dirlight].direction.y, 0.05f, NULL);
  ImGui::DragScalar("z", ImGuiDataType_Float, &ren->dirlights[selected_dirlight].direction.z, 0.05f, NULL);
  ImGui::PopID();


  ImGui::Dummy(ImVec2(0.0f, 20.0f));



  if (ImGui::TreeNode("Point Lights"))
  {
    for (int n = 0; n < NUM_POINTLIGHTS; n++)
    {
      sprintf(buffer, "point light %d", n);
      if (ImGui::Selectable(buffer, selected_pointlight == n))
          selected_pointlight = n;
    }
    ImGui::TreePop();
  }
  

  ImGui::PushID(2);
  ImGui::ColorEdit3("ambient", (float*)&ren->pointlights[selected_pointlight].ambient);
  ImGui::ColorEdit3("diffuse", (float*)&ren->pointlights[selected_pointlight].diffuse);
  ImGui::ColorEdit3("specular", (float*)&ren->pointlights[selected_pointlight].specular);
  
  ImGui::Text("Position");
  ImGui::DragScalar("x", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.x, 0.05f, NULL);
  ImGui::DragScalar("y", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.y, 0.05f, NULL);
  ImGui::DragScalar("z", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.z, 0.05f, NULL);
  ImGui::PopID();


  ImGui::Dummy(ImVec2(0.0f, 20.0f));


  if (ImGui::TreeNode("Spot Lights"))
  {
    for (int n = 0; n < NUM_SPOTLIGHTS; n++)
    {
      sprintf(buffer, "spot light %d", n);
      if (ImGui::Selectable(buffer, selected_spotlight == n))
          selected_spotlight = n;
    }
    ImGui::TreePop();
  }

  ImGui::PushID(3);
  ImGui::ColorEdit3("ambient", (float*)&ren->spotlights[selected_spotlight].ambient);
  ImGui::ColorEdit3("diffuse", (float*)&ren->spotlights[selected_spotlight].diffuse);
  ImGui::ColorEdit3("specular", (float*)&ren->spotlights[selected_spotlight].specular);

  ImGui::DragScalar("intensity", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].intensity, 0.05f, NULL);
  ImGui::DragScalar("inner cutoff", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].inner_cutoff, 0.05f, NULL);
  ImGui::DragScalar("outer cutoff", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].outer_cutoff, 0.05f, NULL);

  ImGui::PushID(4);
  ImGui::Text("Position");
  ImGui::DragScalar("x", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].position.x, 0.05f, NULL);
  ImGui::DragScalar("y", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].position.y, 0.05f, NULL);
  ImGui::DragScalar("z", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].position.z, 0.05f, NULL);
  ImGui::PopID();
  
  ImGui::PushID(5);
  ImGui::Text("Direction");
  ImGui::DragScalar("x", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].direction.x, 0.05f, NULL);
  ImGui::DragScalar("y", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].direction.y, 0.05f, NULL);
  ImGui::DragScalar("z", ImGuiDataType_Float, &ren->spotlights[selected_spotlight].direction.z, 0.05f, NULL);
  ImGui::PopID();
  ImGui::PopID();


}

void draw_render_tab(Renderer *ren)
{
  ImGui::SliderFloat("FOV", &ren->cam.fov, 45.0f, 110.0f);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void draw_physics_tab(Renderer *ren)
{
  ImGui::SliderFloat("Gravity", &ren->gravity, 0, 10);
}

void draw_dev_ui(Renderer *ren)
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  
  ImGui::Begin("Hello, world!");

  if (ImGui::Button("Demo Window"))
    show = !show;
  if (show)
    ImGui::ShowDemoWindow(&show);


  if (ImGui::BeginTabBar("MyTabBar", 0))
  {
    if (ImGui::BeginTabItem("Lighting"))
    {
      draw_lighting_tab(ren);
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Render"))
    {
      draw_render_tab(ren);
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Physics?"))
    {
      draw_physics_tab(ren);
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }




  // if (ImGui::Button("Import"))
  // {
  //   FILE *fh = fopen("export.txt", "r");
  //   fscanf(fh, "%f %f %f\n", &ren->lightsource.ambient.x,  &ren->lightsource.ambient.y,  &ren->lightsource.ambient.z);
  //   fscanf(fh, "%f %f %f\n", &ren->lightsource.diffuse.x,  &ren->lightsource.diffuse.y,  &ren->lightsource.diffuse.z);
  //   fscanf(fh, "%f %f %f\n", &ren->lightsource.specular.x, &ren->lightsource.specular.y, &ren->lightsource.specular.z);
  //   fclose(fh);
  // }
  // if (ImGui::Button("Export"))
  // {
  //   FILE *fh = fopen("export.txt", "w");
  //   fprintf(fh, "%f %f %f\n", ren->lightsource.ambient.x,  ren->lightsource.ambient.y,   ren->lightsource.ambient.z);
  //   fprintf(fh, "%f %f %f\n", ren->lightsource.diffuse.x,  ren->lightsource.diffuse.y,   ren->lightsource.diffuse.z);
  //   fprintf(fh, "%f %f %f\n", ren->lightsource.specular.x, ren->lightsource.specular.y,  ren->lightsource.specular.z);
  //   fclose(fh);
  // }
  //     counter++;
  // ImGui::SameLine();
  // ImGui::Text("counter = %d", counter);

  // ImGui::Text("P(x, y, z): %.2f, %.2f, %.2f", player.pos->x, player.pos->y, player.pos->z);
  // ImGui::Text("V(x, y, z): %.2f, %.2f, %.2f", player.vel.x, player.vel.y, player.vel.z);
  // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();


  ImGui::Render();
}