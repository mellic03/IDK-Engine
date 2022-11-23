#include "ui.h"

int selected_lightsource = 0;
bool show = false;


void draw_lighting_tab(Renderer *ren)
{
  char buffer[64];


  if (ImGui::TreeNode("Point Lights"))
  {
    for (int n = 0; n < 8; n++)
    {
      sprintf(buffer, "point light %d", n);
      if (ImGui::Selectable(buffer, selected_lightsource == n))
          selected_lightsource = n;
    }
    ImGui::TreePop();
  }

  ImGui::ColorEdit3("ambient", (float*)&ren->lightsources[selected_lightsource].ambient);
  ImGui::ColorEdit3("diffuse", (float*)&ren->lightsources[selected_lightsource].diffuse);
  ImGui::ColorEdit3("specular", (float*)&ren->lightsources[selected_lightsource].specular);

  ImGui::DragScalar("x", ImGuiDataType_Float, &ren->lightsources[selected_lightsource].position.x, 0.05f, NULL);
  ImGui::DragScalar("y", ImGuiDataType_Float, &ren->lightsources[selected_lightsource].position.y, 0.05f, NULL);
  ImGui::DragScalar("z", ImGuiDataType_Float, &ren->lightsources[selected_lightsource].position.z, 0.05f, NULL);


  if (ImGui::TreeNode("Spot Lights"))
  {
    for (int n = 0; n < 8; n++)
    {
      sprintf(buffer, "spot light %d", n);
      if (ImGui::Selectable(buffer, selected_lightsource == n))
          selected_lightsource = n;
    }
    ImGui::TreePop();
  }

  ImGui::ColorEdit3("ambient", (float*)&ren->lightsources[selected_lightsource].ambient);
  ImGui::ColorEdit3("diffuse", (float*)&ren->lightsources[selected_lightsource].diffuse);
  ImGui::ColorEdit3("specular", (float*)&ren->lightsources[selected_lightsource].specular);

  ImGui::DragScalar("x", ImGuiDataType_Float, &ren->lightsources[selected_lightsource].position.x, 0.05f, NULL);
  ImGui::DragScalar("y", ImGuiDataType_Float, &ren->lightsources[selected_lightsource].position.y, 0.05f, NULL);
  ImGui::DragScalar("z", ImGuiDataType_Float, &ren->lightsources[selected_lightsource].position.z, 0.05f, NULL);


}

void draw_render_tab(Renderer *ren)
{
  ImGui::SliderFloat("FOV", &ren->cam.fov, 45.0f, 110.0f);
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