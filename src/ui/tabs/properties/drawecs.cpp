#include "drawecs.h"





void DrawECS::pointlight(GameObject *object, PointLight *pointlight)
{
  ImGui::PushID(object->getID());

  if (ImGui::CollapsingHeader("Pointlight"))
  {
    ImGui::Checkbox("Enabled", &pointlight->active);

    bool shadowed = pointlight->shadowmapped;
    ImGui::Checkbox("Shadow mapping", &pointlight->shadowmapped);
    if (shadowed != pointlight->shadowmapped)
      Render::ren.genDepthCubemap(&pointlight->FBO, &pointlight->depthCubemap);

    ImGui::ColorEdit3("ambient", (float*)&pointlight->ambient);
    ImGui::ColorEdit3("diffuse", (float*)&pointlight->diffuse);

    ImGui::DragFloat("radius", &pointlight->radius, 0.1f, 0.0f, 50.0f);
    ImGui::DragFloat("constant", &pointlight->constant, 0.1f, 0.0f, 100.0f);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &pointlight->linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &pointlight->quadratic, 0.001f, 0);
    ImGui::DragScalar("bias", ImGuiDataType_Float, &pointlight->bias,            0.001f, 0);

    ImGui::Checkbox("Volumetrics", &pointlight->volumetrics_active);
    ImGui::DragScalar("fog constant",  ImGuiDataType_Float, &pointlight->fog_constant,    0.001f, 0);
    ImGui::DragScalar("fog linear",    ImGuiDataType_Float, &pointlight->fog_linear,      0.001f, 0);
    ImGui::DragScalar("fog quadratic", ImGuiDataType_Float, &pointlight->fog_quadratic,   0.001f, 0);
    ImGui::DragScalar("fog intensity", ImGuiDataType_Float, &pointlight->fog_intensity,   0.001f, 0);
  }
  ImGui::PopID();
}



void DrawECS::transform(GameObject *object, TransformComponent *transformComponent)
{
  char buffer[64];
  sprintf(buffer, "Transform ##%d", object->getID());

  std::string label = "";
  label += ICON_FA_ARROWS;
  label += "  Transform ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    EngineUI::dragVec3("Position", object->getPos(), 0.0f, 0.0f, 0.01f, "%0.3f", 0.0f);
    EngineUI::dragVec3("Velocity", object->getVel(), 0.0f, 0.0f, 0.01f, "%0.3f", 0.0f);

    glm::quat q = object->getTransform()->orientation;
    glm::vec3 drot1 = glm::degrees(glm::eulerAngles(q));
    glm::vec3 drot2 = drot1;
    EngineUI::dragVec3("Rotation", &drot1, 0.0f, 0.0f, 0.1f, "%0.1f", 0.0f);
    object->getTransform()->addRot(glm::radians(drot1 - drot2));

    EngineUI::dragVec3("Scale", object->getTransform()->getScale(), 0.01f, 10.0f, 0.01f, "%0.3f", 1.0f);
  }
}



void DrawECS::script(GameObject *object, ScriptComponent *scriptComponent)
{
  scriptComponent->script_changed = false;

  std::string label = "";
  label += ICON_FA_FILE_O;
  label += "  Script        " + scriptComponent->script_path.filename().string();
  label += " ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap))
  {
    if (ImGui::Button(std::string(scriptComponent->script_name + ".lua").c_str()))
      ImGui::OpenPopup("Change Script");

    ImGui::SetNextWindowSize({300, 300});
    if (ImGui::BeginPopup("Change Script"))
    {
      EngineUI::draw_directory_recursive(fs::current_path()/"LuaScripting/scripts", &scriptComponent->script_path, &scriptComponent->script_changed);
      
      if (scriptComponent->script_changed)
      {

        fs::path filepath = fs::relative(scriptComponent->script_path, ".");
        scriptComponent->script_name = filepath.string();
        scriptComponent->script_name.erase(scriptComponent->script_name.size() - 4);
      
        scriptComponent->script_path = filepath;


        printf("script_path: %s\n", filepath.c_str());

      }

      ImGui::EndPopup();
    }
  }
}



static glm::vec3 calculate_barycentric(float x, float y, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
  float denom = (v2.y-v3.y)*(v1.x-v3.x) + (v3.x-v2.x)*(v1.y-v3.y);

  glm::vec3 weights;
  weights.x = ((v2.y-v3.y)*(x-v3.x) + (v3.x-v2.x)*(y-v3.y)) / denom;
  weights.y = ((v3.y-v1.y)*(x-v3.x) + (v1.x-v3.x)*(y-v3.y)) / denom;
  weights.z = 1 - weights.x - weights.y;
  return weights;
}



void DrawECS::terrain(GameObject *object, TerrainComponent *terrain_component)
{
  std::string label = "";
  label += ICON_FA_TREE;
  label += "  Terrain ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str()))
  {
    ImGui::DragFloat("threshold", &terrain_component->threshold, 0.001f);
    ImGui::DragFloat("epsilon",   &terrain_component->epsilon,   0.001f);
  
    // High code
    //----------------------------------------------------------------
    static bool first = true;

    if (first)
    {
      if (ImGui::Button("Do the thing"))
      {
        first = false;
        
        int count = 0;

        glm::mat4 model_mat = object->getTransform()->getModelMatrix();

        for (size_t i=0; i<object->getModel()->m_meshes[0].vertices.size(); i+=3)
        {
          Vertex v1 = object->getModel()->m_meshes[0].vertices[i+0];
          Vertex v2 = object->getModel()->m_meshes[0].vertices[i+1];
          Vertex v3 = object->getModel()->m_meshes[0].vertices[i+2];

          if (glm::normalize(v1.normal).y < 0.7f)
            continue;

          
          float xmin = glm::min(v1.position.x, glm::min(v2.position.x, v3.position.x));
          float xmax = glm::max(v1.position.x, glm::max(v2.position.x, v3.position.x));
          float zmin = glm::min(v1.position.z, glm::min(v2.position.z, v3.position.z));
          float zmax = glm::max(v1.position.z, glm::max(v2.position.z, v3.position.z));

          for (int i=0; i<55; i++)
          {
            glm::vec3 midpoint = (v1.position + v2.position + v3.position) / 3.0f;

            float d1 = glm::distance(v1.position, midpoint);
            float d2 = glm::distance(v2.position, midpoint);
            float d3 = glm::distance(v3.position, midpoint);

            float min_vert_dist = glm::min(d1, glm::min(d2, d3));

            float r1 = -min_vert_dist + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(min_vert_dist - -min_vert_dist)));
            float r2 = -min_vert_dist + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(min_vert_dist - -min_vert_dist)));
            

            glm::vec3 weights = calculate_barycentric(
              midpoint.x + r1, midpoint.z + r2,
              glm::vec2(v1.position.x, v1.position.z),
              glm::vec2(v2.position.x, v2.position.z),
              glm::vec2(v3.position.x, v3.position.z)
            );

            if (weights.x < -0.01f || weights.y < -0.01f || weights.z < -0.01f)
              continue;

            float x = weights.x*v1.position.x + weights.y*v2.position.x + weights.z*v3.position.x;
            float y = weights.x*v1.position.y + weights.y*v2.position.y + weights.z*v3.position.y;
            float z = weights.x*v1.position.z + weights.y*v2.position.z + weights.z*v3.position.z;

            
            Transform *transform = new Transform();
            transform->parent = object->getTransform();
            transform->position = glm::vec3(x, y, z);
            Scene::scenegraph.newObjectInstance("grass", transform);
            count += 1;
          }
        }

        auto data = Scene::scenegraph.getInstanceData();
        InstanceData *iData = &data->at("grass");
        iData->genVBO();
        iData->setVertexAttribs();

        printf("Placed %d grass instances\n", count);

      }
    }
    //----------------------------------------------------------------

  }
}



void DrawECS::lod(GameObject *object, LODComponent *lod_component)
{
  std::string label = "";
  label += ICON_FA_EYE;
  label += "  Level of Detail ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::Checkbox("Override Global", &object->getLODData()->override_global_lod);

    if (object->getLODData()->override_global_lod)
    {
      const int max_lod = object->getLevelsLOD() - 1;
      ImGui::SliderInt("LOD", object->getLOD_value(), 0, max_lod);
    }
  }
}



void DrawECS::sphere(GameObject *object, SphereColliderComponent *sphereColliderComponent)
{
  std::string label = "";
  label += ICON_COMPONENT_SPHERE;
  label += "  Sphere Collider ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::Checkbox("Visualise", &sphereColliderComponent->visualise);
    ImGui::DragFloat("Radius", &sphereColliderComponent->sphere_collider->radius);
  }
}



void DrawECS::capsule(GameObject *object, CapsuleColliderComponent *capsuleColliderComponent)
{
  std::string label = "";
  label += ICON_COMPONENT_SPHERE;
  label += "  Capsule Collider ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::Checkbox("Visualise", &capsuleColliderComponent->visualise);
    ImGui::DragFloat("Radius", &capsuleColliderComponent->capsule_collider->radius);
    ImGui::DragFloat("Top", &capsuleColliderComponent->capsule_collider->top);
    ImGui::DragFloat("Bottom", &capsuleColliderComponent->capsule_collider->bottom);
  }
}



void DrawECS::animation(GameObject *object)
{
  std::string label = "";
  label += ICON_FA_FILM;
  label += "  Animation ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {

    std::vector<std::string> animation_names;

    Animation::AnimationController *animationController = object->getAnimationController();
    std::map<std::string, Animation::Animation> *animations = animationController->getAnimations();
    for (auto it = animations->begin(); it != animations->end(); it++)
    {
      Animation::Animation *animation = &(it->second);
      animation_names.push_back(animation->getName());
    }

    if (animation_names.size() == 0)
      return;

    static int item_current_idx = 0;
    const char* combo_preview_value = animation_names[item_current_idx].c_str();
    if (ImGui::BeginCombo("combo 1", combo_preview_value, 0))
    {
      for (size_t n = 0; n < animation_names.size(); n++)
      {
        const bool is_selected = (item_current_idx == n);
        if (ImGui::Selectable(animation_names[n].c_str(), is_selected))
          item_current_idx = n;

        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    
    animationController->setActiveAnimation(animation_names[item_current_idx]);
  }
}


