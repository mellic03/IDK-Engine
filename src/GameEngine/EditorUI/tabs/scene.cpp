#include "../UIEngine.h"



static void draw_submenu(const char *title, SceneGraph *scenegraph, int *selected_instance, std::list<GameObject *> *object_templates)
{
  if (ImGui::BeginMenu(title))
  {
    for (GameObject *obj: *object_templates)
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


static void draw_new_instance_menu(int *selected_instance)
{
  if (ImGui::BeginPopupContextWindow("New Instance"))
  {
    ImGui::Text("New Instance");
    ImGui::Separator();

    SceneGraph *scenegraph = &Scene::scenegraph;

    draw_submenu("Terrain",       scenegraph, selected_instance, scenegraph->getTemplatesByType(GAMEOBJECT_TERRAIN));
    draw_submenu("Static",        scenegraph, selected_instance, scenegraph->getTemplatesByType(GAMEOBJECT_STATIC));
    draw_submenu("Billboard",     scenegraph, selected_instance, scenegraph->getTemplatesByType(GAMEOBJECT_BILLBOARD));
    draw_submenu("Actor",         scenegraph, selected_instance, scenegraph->getTemplatesByType(GAMEOBJECT_ACTOR));
    draw_submenu("Lightsource",   scenegraph, selected_instance, scenegraph->getTemplatesByType(GAMEOBJECT_LIGHTSOURCE));
    draw_submenu("Misc",          scenegraph, selected_instance, scenegraph->getTemplatesByType(GAMEOBJECT_EMPTY));

    ImGui::EndPopup();
  }
}


static void draw_entity_childnodes(GameObject *object, int *selected_instance)
{
  ImGui::PushID(object->getID());

  int flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

  if (object->hasChildren() == false)
    flags |= ImGuiTreeNodeFlags_Leaf;

  if (*selected_instance == object->getID())
    flags |= ImGuiTreeNodeFlags_Selected;


  std::string label = "";
  label += EngineUI::getObjectIcon(object->getData()->getUiIconType());
  label += "  " + object->getName();

  if (ImGui::TreeNodeEx(label.c_str(), flags))
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
        object->giveChild(Scene::scenegraph.objectPtr(selectedobj));
      }
      ImGui::EndDragDropTarget();
    }

    std::vector<GameObject *> children = object->getChildren();
    for (size_t i=0; i<children.size(); i++)
    {
      draw_entity_childnodes(children[i], selected_instance);
    }

    ImGui::TreePop();
  }

  ImGui::PopID();
}


void EngineUI::sceneHierarchy(Renderer *ren)
{
  ImGui::Begin("Scene Hierarchy");


  draw_new_instance_menu(&EngineUI::selected_objectID);
  
  if (ImGui::TreeNodeEx("Scene Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
  {
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
      {
        IM_ASSERT(payload->DataSize == sizeof(int));
        int selectedobj = *(int *)payload->Data;
        Scene::scenegraph.objectPtr(EngineUI::selected_objectID)->clearParent();
      }
      ImGui::EndDragDropTarget();
    }

    for (auto &object: Scene::scenegraph.m_selectable_instances)
      if (object->hasParent() == false)
      {
        draw_entity_childnodes(object, &EngineUI::selected_objectID);

        // if (object.getTemplateName() == "wall")
        //   break;
      }

    ImGui::TreePop();
  }

  ImGui::End();
}





