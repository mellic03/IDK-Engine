#include <string>
#include <iostream>
#include <filesystem>

#include "ui.h"

namespace fs = std::filesystem;


void draw_save_modal(bool draw)
{
  if (draw)
    ImGui::OpenPopup("Save Scene");

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Save Scene", NULL, 0))
  {
    static fs::path workingdir = fs::current_path();
    fs::create_directories(workingdir / "assets" / "scenes" );
    static fs::path scene_path = workingdir / "assets" / "scenes";
    static fs::path save_path;

    bool changed = false;
    EngineUI::draw_directory_recursive(workingdir / "assets" / "scenes", &save_path, &changed);
    if (changed)
    {
      printf("WEE\n");
    }

    static std::string filename;

    if (save_path.has_extension())
    {
      filename = save_path.filename().string();
      save_path = save_path.parent_path();
    }

    ImGui::InputText("Filename", &filename, 64);
    std::string save_filepath = save_path.string() + "/" + filename;
    std::string relative_save_path = fs::relative(save_path, fs::current_path()).string() + "/" + filename;
    ImGui::Text("Saving under: %s", relative_save_path.c_str());


    if (ImGui::Button("Save", ImVec2(120, 0)))
    {
      std::cout << "exporting scene to: " << relative_save_path << std::endl;
      Scene::scenegraph.exportScene(relative_save_path);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", ImVec2(120, 0)))
      ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}


void draw_load_modal(bool draw)
{
  if (draw)
    ImGui::OpenPopup("Load Scene");

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Load Scene", NULL, 0))
  {
    static fs::path selected_filepath;
    ImGui::BeginChild("directory tree", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y*0.8), true);
    {
      static const std::filesystem::path workingdir = fs::current_path();
      std::filesystem::create_directories(workingdir / "assets" / "scenes" );
      static std::filesystem::path scenedir = workingdir / "assets" / "scenes";
      bool wee;
      EngineUI::draw_directory_recursive(scenedir, &selected_filepath, &wee);

      ImGui::EndChild();
    }

    ImGui::Text("File: %s", selected_filepath.filename().c_str());

    
    std::string load_path = fs::relative(selected_filepath, fs::current_path()).string();

    if (ImGui::Button("Load", ImVec2(120, 0)))
    {
      Scene::importScene(load_path.c_str());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
      ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}


void draw_main_menu_bar()
{
  bool show_save_modal = false;
  bool show_load_modal = false;
  static bool show = false;

  if (ImGui::BeginMainMenuBar())
  {

    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New", "CTRL+N"))
        Scene::defaultScene();

      if (ImGui::MenuItem("Save", "CTRL+S"))
        show_save_modal = true;

      if (ImGui::MenuItem("Load", "CTRL+O"))
        show_load_modal = true;

      ImGui::EndMenu();
    }


    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Demo Window", ""))
        show = !show;


      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {}
      if (ImGui::MenuItem("Copy", "CTRL+C")) {}
      if (ImGui::MenuItem("Paste", "CTRL+V")) {}
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Engine"))
    {
      if (ImGui::MenuItem("Clear Model Cache", "CTRL+M", false, false))
      {
        
      }
      ImGui::EndMenu();
    }


    ImGui::EndMainMenuBar();
  }

  draw_save_modal(show_save_modal);
  draw_load_modal(show_load_modal);

  if (show)
    ImGui::ShowDemoWindow(&show);
}