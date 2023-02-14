#include "../UIEngine.h"


static void drawBoundingSphereHierarchy(BVNode *node)
{
  if (ImGui::TreeNodeEx(node->objectptr->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
  {
    if (node->left != nullptr)
      drawBoundingSphereHierarchy(node->left);

    ImGui::TreePop();
  }

  if (node->right != nullptr)
    drawBoundingSphereHierarchy(node->right);

}


void EngineUI::debug(Renderer *ren)
{
  ImGui::Begin("Debug");

  ImGui::Text("Debug");
  ImGui::Separator();


  EngineUI::bitFlagCheckbox(
    "Draw sphere colliders",
    static_cast<unsigned char>(RenderDebugFlag::DrawColliders),
    (unsigned char *)(ren->getDebugData()->getDebugFlags())
  );


  EngineUI::bitFlagCheckbox(
    "Draw bounding spheres",
    static_cast<unsigned char>(RenderDebugFlag::DrawBoundingSpheres),
    (unsigned char *)(ren->getDebugData()->getDebugFlags())
  );


  
  BVNode *root = Scene::scenegraph.getBVTree()->getRootNode();
  if (root != nullptr)
  {
    drawBoundingSphereHierarchy(root);
  }  


  ImGui::End();
}

