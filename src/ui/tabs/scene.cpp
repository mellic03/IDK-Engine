#include "../UIEngine.h"


static void draw_new_instance_menu(SceneGraph *scenegraph, int *selected_instance)
{
  if (ImGui::BeginPopupContextWindow("New Instance"))
  {
    ImGui::Text("New Instance");
    ImGui::Separator();
    
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

    ImGui::EndPopup();
  }
}


static void draw_entity_childnodes(SceneGraph *scenegraph, GameObject *object, int *selected_instance)
{
  ImGui::PushID(object->getID());

  int flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

  if (object->hasChildren() == false)
    flags |= ImGuiTreeNodeFlags_Leaf;

  if (*selected_instance == object->getID())
    flags |= ImGuiTreeNodeFlags_Selected;

  if (ImGui::TreeNodeEx(object->m_given_name.c_str(), flags))
  {
    if (ImGui::IsItemClicked())
      *selected_instance = object->getID();


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

    std::vector<GameObject *> children = object->getChildren();
    for (int i=0; i<children.size(); i++)
    {
      draw_entity_childnodes(scenegraph, children[i], selected_instance);
    }

    ImGui::TreePop();
  }

  ImGui::PopID();
}


void EngineUI::sceneHierarchy(Renderer *ren, Scene *scene)
{
  ImGui::Begin("Scene Hierarchy");
  
  draw_new_instance_menu(scene->m_scenegraph, &EngineUI::selected_objectID);

  
  if (ImGui::TreeNodeEx("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
  {
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
      {
        IM_ASSERT(payload->DataSize == sizeof(int));
        int selectedobj = *(int *)payload->Data;
        scene->m_scenegraph->objectPtr(EngineUI::selected_objectID)->clearParent();
      }
      ImGui::EndDragDropTarget();
    }

    for (auto &object: scene->m_scenegraph->m_object_instances)
      if (object.hasParent() == false)
        draw_entity_childnodes(scene->m_scenegraph, &object, &EngineUI::selected_objectID);

    ImGui::TreePop();
  }

  ImGui::End();
}





