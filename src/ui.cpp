#include "scene.h"
#include "ui.h"
#include "stdio.h"

int selected_dirlight = 0;    const char *dir_options[4] = {"1"};
int selected_pointlight = 0;  const char *point_options[4] = {"1", "2", "3", "4"};
int selected_spotlight = 0;   const char *spot_options[4] = {"1", "2"};
bool show = false;


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
    fprintf(fh, "POINTLIGHT POSITION: %f %f %f\n", light->position.x, light->position.y, light->position.z);
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
    fscanf(fh, "POINTLIGHT POSITION: %f %f %f\n", &light->position.x, &light->position.y, &light->position.z);
  }

  fscanf(fh, "CLEAR COLOR: %f %f %f\n", &ren->clearColor.x, &ren->clearColor.y, &ren->clearColor.z);
  fscanf(fh, "FOG START, STOP: %f %f\n", &ren->fog_start, &ren->fog_end);
  fscanf(fh, "FOV: %f\n", &ren->fov);
  fscanf(fh, "EXPOSURE: %f\n", &ren->exposure);
  fscanf(fh, "GAMMA: %f\n", &ren->gamma);

  fclose(fh);
}
 
 
void draw_transform_menu(Scene *scene, GameObject *object, NavMesh *navmesh)
{
  ImGui::PushID(1);
  ImGui::Text("Position");
  ImGui::DragScalar("x", ImGuiDataType_Float, &object->pos.x, 0.05f, 0);
  ImGui::DragScalar("y", ImGuiDataType_Float, &object->pos.y, 0.05f, 0);
  ImGui::DragScalar("z", ImGuiDataType_Float, &object->pos.z, 0.05f, 0);
  ImGui::PopID();

  ImGui::PushID(2);
  ImGui::Text("Velocity");
  ImGui::DragScalar("x", ImGuiDataType_Float, &object->vel.x, 0.05f, 0);
  ImGui::DragScalar("y", ImGuiDataType_Float, &object->vel.y, 0.05f, 0);
  ImGui::DragScalar("z", ImGuiDataType_Float, &object->vel.z, 0.05f, 0);
  // ImGui::SliderFloat("x", &object->vel.x, -100.0f, 100.0f, "%0.4f", 0);
  // ImGui::SliderFloat("y", &object->vel.y, -100.0f, 100.0f, "%0.4f", 0);
  // ImGui::SliderFloat("z", &object->vel.z, -100.0f, 100.0f, "%0.4f", 0);
  ImGui::PopID();

  ImGui::PushID(3);
  ImGui::Text("Rotation");
  ImGui::DragScalar("x", ImGuiDataType_Float, &object->model->rot.x, 0.5f, 0);
  ImGui::DragScalar("y", ImGuiDataType_Float, &object->model->rot.y, 0.5f, 0);
  ImGui::DragScalar("z", ImGuiDataType_Float, &object->model->rot.z, 0.5f, 0);
  ImGui::Checkbox("local", &object->model->rotate_local);

  if (object->model->rot.x >= 360) object->model->rot.x -= 360;
  if (object->model->rot.y >= 360) object->model->rot.y -= 360;
  ImGui::PopID();
  
  ImGui::PushID(4);
  static glm::vec3 movetowards;
  ImGui::DragScalar("x", ImGuiDataType_Float, &movetowards.x, 0.5f, 0);
  ImGui::DragScalar("y", ImGuiDataType_Float, &movetowards.y, 0.5f, 0);
  ImGui::DragScalar("z", ImGuiDataType_Float, &movetowards.z, 0.5f, 0);
  if (ImGui::Button("Move"))
  {
    object->move_towards = movetowards;
    object->path = navmesh->path(object->pos, *scene->player->pos);
    object->changeState(GSTATE_MOVETOWARDS);
  }
  ImGui::PopID();
}

void draw_entities_tab(Renderer *ren, Scene *scene)
{
  ImGui::Dummy(ImVec2(0.0f, 20.0f));

  std::vector<std::string> model_names;



  for (int i=0; i<scene->renderables.objects.size(); i++)
  {
    char buffer[32];
    sprintf(buffer, "%s", scene->renderables.objects[i]->model->m_name.c_str());

    ImGui::PushID(i);
    if (ImGui::TreeNode(buffer))
    {
      
      if (ImGui::TreeNode("Transform"))
      {
        draw_transform_menu(scene, scene->renderables.objects[i], &scene->navmesh);

        ImGui::TreePop();
      }


      ImGui::TreePop();
    }
    ImGui::PopID();
  }


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

    ImGui::DragScalar("bias", ImGuiDataType_Float, &ren->DIRBIAS, 0.05f, NULL);
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

    ImGui::Text("Position");
    ImGui::DragScalar("x", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.x, 0.05f, 0);
    ImGui::DragScalar("y", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.y, 0.05f, 0);
    ImGui::DragScalar("z", ImGuiDataType_Float, &ren->pointlights[selected_pointlight].position.z, 0.05f, 0);
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

  ImGui::EndTable();
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

void draw_dev_ui(Renderer *ren, Scene *scene)
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
    if (ImGui::BeginTabItem("Entities"))
    {
      draw_entities_tab(ren, scene);
      ImGui::EndTabItem();
    }

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
    if (ImGui::BeginTabItem("Physics"))
    {
      draw_physics_tab(ren);
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }


  ImGui::BeginChild("RenderWindow");

  ImVec2 wsize = ImGui::GetWindowSize();
  ImGui::Image((ImTextureID)ren->depthMap, wsize, ImVec2(0, 1), ImVec2(1, 0));

  ImGui::EndChild();
  

  ImGui::End();
  ImGui::Render();

}