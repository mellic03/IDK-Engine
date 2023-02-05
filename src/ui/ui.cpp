#include <stdio.h>

#include "UIEngine.h"

#include "ui.h"

#include <fstream>
#include <sstream>

int selected_dirlight = 0;    const char *dir_options[4] = {"1"};
int selected_pointlight = 0;  const char *point_options[4] = {"1", "2", "3", "4"};
int selected_spotlight = 0;   const char *spot_options[4] = {"1", "2"};


void draw_lighting_tab(Renderer *ren)
{
  ImGui::Text("Directional Light");
  ImGui::Separator();
  ImGui::DragFloat3("Position",       &Scene::scenegraph.dirlight.position[0], 0.1f);
  ImGui::ColorEdit3("Diffuse",        &Scene::scenegraph.dirlight.diffuse[0], 0.1f);
  ImGui::ColorEdit3("Ambient",        &Scene::scenegraph.dirlight.ambient[0], 0.1f);
  ImGui::DragFloat("Bias",            &Scene::scenegraph.dirlight.bias, 0.01f);
  ImGui::DragFloat("Fog intensity",   &Scene::scenegraph.dirlight.fog_intensity, 0.01f);
  ImGui::Dummy(ImVec2(0.0f, 20.0f));


  ImGui::Text("Bloom");
  ImGui::Separator();
  ImGui::DragFloat("Threshold", &ren->bloom.threshold, 0.01f, 0.0f, 10.0f);
  ImGui::DragFloat("Strength", &ren->bloom.strength, 0.0001f, 0.0f, 1.0f);
  
  int min_res = ren->bloom.min_downsample_resolution;
  ImGui::DragInt("min downsampling resolution", &ren->bloom.min_downsample_resolution, 0.01f, 1, 64);
  if (min_res != ren->bloom.min_downsample_resolution)
    ren->resize(ren->viewport_width, ren->viewport_height);  


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
}


static void draw_framebuffers2(Renderer *ren, ImVec2 viewportsize, std::string name1, GLuint fb1, std::string name2, GLuint fb2)
{
  ImGui::BeginGroup();
  ImGui::Text(name1.c_str());
  ImGui::Image(*(ImTextureID *)(void *)&fb1, {viewportsize.x/2.0f, viewportsize.y/2.0f}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  ImGui::EndGroup();

  ImGui::SameLine();

  ImGui::BeginGroup();
  ImGui::Text(name2.c_str());
  ImGui::Image(*(ImTextureID *)(void *)&fb2, {viewportsize.x/2.0f, viewportsize.y/2.0f}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  ImGui::EndGroup();
}


static void draw_framebuffers3(Renderer *ren, ImVec2 viewportsize, std::string name1, GLuint fb1, std::string name2, GLuint fb2, std::string name3, GLuint fb3)
{
  ImGui::BeginGroup();
  ImGui::Text(name1.c_str());
  ImGui::Image(*(ImTextureID *)(void *)&fb1, {viewportsize.x/3.0f, viewportsize.y/3.0f}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  ImGui::EndGroup();

  ImGui::SameLine();

  ImGui::BeginGroup();
  ImGui::Text(name2.c_str());
  ImGui::Image(*(ImTextureID *)(void *)&fb2, {viewportsize.x/3.0f, viewportsize.y/3.0f}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  ImGui::EndGroup();

  ImGui::SameLine();

  ImGui::BeginGroup();
  ImGui::Text(name3.c_str());
  ImGui::Image(*(ImTextureID *)(void *)&fb3, {viewportsize.x/3.0f, viewportsize.y/3.0f}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  ImGui::EndGroup();
}


static void draw_framebuffers_n(Renderer *ren, ImVec2 viewportsize, int n, std::vector<std::string> names, std::vector<GLuint> framebuffer_IDs)
{
  float w = viewportsize.x / n;
  float h = viewportsize.y / n;


  for (size_t i=0; i<names.size(); i++)
  {
    ImGui::BeginGroup();
    ImGui::Text(names[i].c_str());
    ImGui::Image(*(ImTextureID *)(void *)&framebuffer_IDs[i], ImVec2(w, h), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::EndGroup();

    ImGui::SameLine();
  }

  ImGui::Dummy(ImVec2(0.0f, 0.0f));

}


void draw_framebuffers(Renderer *ren)
{
  ImGui::Begin("Frame Buffers");
  {
    ImVec2 viewportsize = ImGui::GetContentRegionAvail();
    
    draw_framebuffers_n( ren, viewportsize, 4,
                         {"position",            "normal",            "albedo",                "emission"},
                         {ren->gbuffer_position, ren->gbuffer_normal, ren->gbuffer_albedospec, ren->gbuffer_emission}
                      );

    draw_framebuffers_n( ren, viewportsize, 2,
                         {"general 0",                 "general 1"},
                         {ren->generalColorBuffers[0],  ren->generalColorBuffers[1]}
                       );

    draw_framebuffers_n( ren, viewportsize, 2,
                         {"ping pong 0",                 "ping pong 1"},
                         {ren->pingPongColorBuffers[0],  ren->pingPongColorBuffers[1]}
                      );

    draw_framebuffers_n( ren, viewportsize, 2,
                         {"Screen Quad 0",                 "Screen Quad 1"},
                         {ren->screenQuadColorBuffers[0],  ren->screenQuadColorBuffers[1]}
                      );

    draw_framebuffers_n( ren, viewportsize, 4,
                         {"0",     "1",     "2",     "3"},
                         {ren->dirlight_depthmapArray[0],  ren->dirlight_depthmapArray[1], ren->dirlight_depthmapArray[2], ren->dirlight_depthmapArray[3]}
                      );

    draw_framebuffers_n( ren, viewportsize, 4,
                         {"4",     "5",     "6",     "7"},
                         {ren->dirlight_depthmapArray[4],  ren->dirlight_depthmapArray[5], ren->dirlight_depthmapArray[6], ren->dirlight_depthmapArray[7]}
                      );

    for (int i=0; i<ren->num_blur_FBOs; i+=2)
    {
      ImGui::BeginGroup();
      ImGui::Text("Blur Framebuffer %d", i);
      ImGui::Image(*(ImTextureID *)(void *)&ren->blurColorBuffers[i], {viewportsize.x/2, viewportsize.y/2}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();

      ImGui::SameLine();

      ImGui::BeginGroup();
      ImGui::Text("Blur Framebuffer %d", i+1);
      ImGui::Image(*(ImTextureID *)(void *)&ren->blurColorBuffers[i+1], {viewportsize.x/2, viewportsize.y/2}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
      ImGui::EndGroup();
    }



    ImGui::End();
  }
}


void draw_ui_dev2(Renderer *ren, int *x, int *y, int *w, int *h, ImGuiWindowFlags windowflags)
{
  ImGui::Begin("Root", NULL, windowflags);
  {
    ImGui::DockSpace(ImGui::GetID("dock"));

    EngineUI::sceneHierarchy(ren);
    EngineUI::instanceProperties();
    EngineUI::globalProperties();
    EngineUI::scriptBrowser();
    EngineUI::scriptEditor();
    EngineUI::details();
    EngineUI::debug(ren);
    EngineUI::physics();
    EngineUI::render(ren);

    draw_framebuffers(ren);

    ImGui::Begin("Lighting");
    {
      draw_lighting_tab(ren);
      ImGui::End();
    }
  
    ImGui::Begin("Viewport");
    {
      *w = (int)ImGui::GetContentRegionAvail().x;
      *h = (int)ImGui::GetContentRegionAvail().y;

      if (ren->viewport_width != *w || ren->viewport_height != *h)
        ren->resize(*w, *h);

      ImGui::Image(*(ImTextureID *)(void *)&ren->screenQuadColorBuffers[0], ImGui::GetContentRegionAvail(), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
  
      ImGui::End();
    }


    ImGui::End();
  }
}



void draw_ui(Renderer *ren, int *x, int *y, int *w, int *h)
{
  draw_main_menu_bar();

  ImGuiWindowFlags windowflags = 0;
  windowflags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  windowflags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing;
  windowflags |= ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_MenuBar;
  windowflags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

  draw_ui_dev2(ren, x, y, w, h, windowflags);
}
