#include <string>
#include <iostream>
#include <filesystem>

#include "ui.h"

namespace fs = std::filesystem;

// void draw_directory(fs::path pth, fs::path *selected_filepath)
// {
//   for (auto const& dir_entry : std::filesystem::directory_iterator{pth}) 
//   {
//     if (dir_entry.path().has_extension())
//     {
//       ImGui::Text(dir_entry.path().filename().c_str());
//       if (ImGui::IsItemClicked())
//         *selected_filepath = dir_entry.path();
//       continue;  
//     }
    
//     if (ImGui::TreeNode(dir_entry.path().filename().c_str()))
//     {
//       draw_directory(pth / dir_entry.path(), selected_filepath);
//       ImGui::TreePop();
//     }
//   }
// }


// void draw_save_modal(bool draw, Scene *scene)
// {
//   if (draw)
//     ImGui::OpenPopup("Save Scene");

//   ImVec2 center = ImGui::GetMainViewport()->GetCenter();
//   ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//   if (ImGui::BeginPopupModal("Save Scene", NULL, 0))
//   {
//     static const fs::path workingdir = fs::current_path();
//     fs::create_directories(workingdir / "assets" / "scenes" );
//     static const fs::path save_path = workingdir / "assets" / "scenes";

//     static char buf1[64] = "scene1.scene"; ImGui::InputText("Filename", buf1, 64);
//     std::string save_filepath = save_path.string() + "/" + std::string(buf1);
//     ImGui::Text("Saving under: /assets/scenes/%s", buf1);


//     if (ImGui::Button("Save", ImVec2(120, 0)))
//     {
//       scene->object_handler->exportScene(save_filepath.c_str());
//       ImGui::CloseCurrentPopup();
//     }

//     ImGui::SameLine();
//     if (ImGui::Button("Cancel", ImVec2(120, 0)))
//       ImGui::CloseCurrentPopup();

//     ImGui::EndPopup();
//   }
// }

// void draw_load_modal(bool draw, Scene *scene)
// {
//   if (draw)
//     ImGui::OpenPopup("Load Scene");

//   ImVec2 center = ImGui::GetMainViewport()->GetCenter();
//   ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//   if (ImGui::BeginPopupModal("Load Scene", NULL, 0))
//   {
//     static fs::path selected_filepath;
//     ImGui::BeginChild("directory tree", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y*0.8), true);
//     {
//       static const std::filesystem::path workingdir = fs::current_path();
//       std::filesystem::create_directories(workingdir / "assets" / "scenes" );
//       static std::filesystem::path scenedir = workingdir / "assets" / "scenes";
//       draw_directory(scenedir, &selected_filepath);

//       ImGui::EndChild();
//     }

//     ImGui::Text("File: %s", selected_filepath.filename().c_str());

//     std::string load_path = std::string("assets/scenes/" + selected_filepath.filename().string());

//     if (ImGui::Button("Load", ImVec2(120, 0)))
//     {
//       scene->object_handler->importScene(load_path.c_str());
//       scene->player->setObjectPtr(scene->object_handler->rearObjectPtr());
//       ImGui::CloseCurrentPopup();
//     }

//     ImGui::SameLine();
//     if (ImGui::Button("Cancel", ImVec2(120, 0)))
//       ImGui::CloseCurrentPopup();

//     ImGui::EndPopup();
//   }
// }


// void draw_main_menu_bar(Renderer *ren, Scene *scene)
// {

//   bool show_save_modal = false;
//   bool show_load_modal = false;


//   if (ImGui::BeginMainMenuBar())
//   {
//     if (ImGui::BeginMenu("File"))
//     {
//       if (ImGui::MenuItem("Save", "CTRL+S"))
//         show_save_modal = true;

//       if (ImGui::MenuItem("Load", "CTRL+O"))
//         show_load_modal = true;

//       ImGui::EndMenu();
//     }


//     if (ImGui::BeginMenu("Edit"))
//     {
//       if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
//       if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
//       ImGui::Separator();
//       if (ImGui::MenuItem("Cut", "CTRL+X")) {}
//       if (ImGui::MenuItem("Copy", "CTRL+C")) {}
//       if (ImGui::MenuItem("Paste", "CTRL+V")) {}
//       ImGui::EndMenu();
//     }

//     ImGui::EndMainMenuBar();
//   }

//   draw_save_modal(show_save_modal, scene);
//   draw_load_modal(show_load_modal, scene);




// }