#include "../UIEngine.h"



static void draw_submenu(const char *title, SceneGraph *scenegraph, int *selected_instance, std::list<GameObject *> &object_templates)
{
  if (ImGui::BeginMenu(title))
  {
    for (GameObject *obj: object_templates)
    {
      if (ImGui::MenuItem(obj->getTemplateName().c_str()))
      {
        scenegraph->newObjectInstance(obj->getTemplateName());
        *selected_instance = scenegraph->m_object_instances.size() - 1;
      }
    }
    ImGui::EndMenu();
  }
}



static void draw_new_instance_menu(SceneGraph *scenegraph, int *selected_instance)
{
  if (ImGui::BeginPopupContextWindow("New Instance"))
  {
    ImGui::Text("New Instance");
    ImGui::Separator();

    draw_submenu("Terrain Objects",       scenegraph, selected_instance, scenegraph->m_terrain_templates);
    draw_submenu("Static Objects",        scenegraph, selected_instance, scenegraph->m_static_templates);
    draw_submenu("Billboard Objects",     scenegraph, selected_instance, scenegraph->m_billboard_templates);
    draw_submenu("Actor Objects",         scenegraph, selected_instance, scenegraph->m_actor_templates);
    draw_submenu("Lightsource Objects",   scenegraph, selected_instance, scenegraph->m_lightsource_templates);

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
    for (size_t i=0; i<children.size(); i++)
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
      {
        draw_entity_childnodes(scene->m_scenegraph, &object, &EngineUI::selected_objectID);

        // if (object.getTemplateName() == "wall")
        //   break;
      }

    ImGui::TreePop();
  }

  ImGui::End();
}





