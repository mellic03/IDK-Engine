#include "ui.h"
#include "../include/imgui/imgui_stdlib.h"


void draw_properties_menu(Scene *scene, SceneGraph *handler, int selected_instance)
{
  if (handler->m_object_instances.size() == 0)
    return;

  GameObject *object = handler->objectPtr(selected_instance);

  ImGui::InputText("Name", &object->m_given_name);

  ImGui::InputText("Script", &object->m_script_name);

  ImGui::Separator();

  ImGui::Text("Transform");
  ImGui::Separator();
  ImGui::DragFloat3("Position", &object->getPos()->x, 0.01f, 0, 0, "%0.01f", 0);
  ImGui::DragFloat3("Velocity", &object->getVel()->x, 0.01f, 0, 0, "%0.01f", 0);
  ImGui::DragFloat3("Rotation", &object->getRot()->x, 0.1f,  0, 0, "%0.1f", 0);

  ImGui::Dummy(ImVec2(0.0f, 20.0f));
  ImGui::Text("Other Stuff");
  ImGui::Separator();
  ImGui::Checkbox("Hidden", object->isHiddenPtr());

  ImGui::Dummy(ImVec2(0.0f, 20.0f));
  ImGui::Text("Info");
  ImGui::Separator();

  ImGui::Text("environmental:       %s", (object->isEnvironmental() ? "true" : "false"));
  ImGui::Text("physics_state:       %s", object->physStateString().c_str());
  ImGui::Text("navigation_state:    %s", object->navStateString().c_str());
  ImGui::Text("has collision mesh:  %s", object->hasCollisionMesh() ? "true": "false");

  ImGui::Text("\nLua access: worldData.attribute[%d]", selected_instance);




  // if (object->isNPC())
  //   if (ImGui::Button("Seek Player"))
  //     object.setPath(scene->navmesh.path(object->pos_worldspace, scene->player->pos_worldspace));
}


static void draw_new_instance_menu(SceneGraph *scenegraph, int *selected_instance)
{
  if (ImGui::BeginMenu("New Instance"))
  {
    if (ImGui::BeginMenu("Environment"))
    {
      int iterator = 0;
      for (auto &objtemplate: scenegraph->m_object_templates)
      {
        if (objtemplate.isEnvironmental() == false)
          continue;
        
        if (ImGui::MenuItem(objtemplate.getTemplateName().c_str()))
        {
          scenegraph->newObjectInstance(objtemplate.getTemplateName());
          *selected_instance = scenegraph->m_object_instances.size() - 1;
        }

        iterator += 1;
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("NPC"))
    {
      int iterator = 0;
      for (auto &objtemplate: scenegraph->m_object_templates)
      {
        if (objtemplate.isNPC() == false)
          continue;
        
        if (ImGui::MenuItem(objtemplate.getTemplateName().c_str()))
        {
          scenegraph->newObjectInstance(objtemplate.getTemplateName());
          *selected_instance = scenegraph->m_object_instances.size() - 1;
        }

        iterator += 1;
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Misc"))
    {
      for (auto &objtemplate: scenegraph->m_object_templates)
      {

        if (objtemplate.isMisc() == false)
          continue;
  
        if (ImGui::MenuItem(objtemplate.getTemplateName().c_str()))
        {
          scenegraph->newObjectInstance(objtemplate.getTemplateName());
          *selected_instance = scenegraph->m_object_instances.size() - 1;
        }
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenu();
  }
}


static void draw_entity_childnodes(SceneGraph *scenegraph, GameObject *object, int *selected_instance)
{
  ImGui::PushID(object->getID());

  int flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

  if (*selected_instance == object->getID())
    flags |= ImGuiTreeNodeFlags_Selected;

  if (ImGui::TreeNodeEx(object->m_given_name.c_str(), flags))
  {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
      ImGui::SetDragDropPayload("DND_DEMO_CELL", (const void *)object->getIDptr(), sizeof(int));

      ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
      {
        IM_ASSERT(payload->DataSize == sizeof(int));
        int selectedobj = *(int *)payload->Data;
        object->giveChild(scenegraph->objectPtr(selectedobj));
      }
      ImGui::EndDragDropTarget();
    }


    if (ImGui::IsItemClicked())
    {
      *selected_instance = object->getID();
    }

    std::vector<GameObject *> children = object->getChildren();
    for (int i=0; i<children.size(); i++)
    {
      draw_entity_childnodes(scenegraph, children[i], selected_instance);
    }


    ImGui::TreePop();
  }

  ImGui::PopID();
}


void draw_scene_tab(Renderer *ren, Scene *scene, int *selected_instance)
{

  ImGui::BeginChild("entity-child", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50), false, 0);
  {
    // ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x/3, 0), false, 0);
    {
      if (ImGui::TreeNodeEx("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
      {
        if (ImGui::BeginDragDropTarget())
        {
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
          {
            IM_ASSERT(payload->DataSize == sizeof(int));
            int selectedobj = *(int *)payload->Data;
            scene->m_scenegraph->objectPtr(*selected_instance)->clearParent();
          }
          ImGui::EndDragDropTarget();
        }

        for (auto &object: scene->m_scenegraph->m_object_instances)
          if (object.hasParent() == false)
            draw_entity_childnodes(scene->m_scenegraph, &object, selected_instance);

        ImGui::TreePop();
      }

      // ImGui::EndChild();
    }

    ImGui::SameLine();

    // ImGui::BeginChild("ChildR", ImVec2(ImGui::GetContentRegionAvail().x, 0), true, 0);
    // {
    //   draw_properties_menu(scene, scene->m_scenegraph, *selected_instance);  
    //   ImGui::EndChild();
    // }

    ImGui::EndChild();
  }
  draw_new_instance_menu(scene->m_scenegraph, selected_instance);
}
