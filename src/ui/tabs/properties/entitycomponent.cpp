#include "../../UIEngine.h"
#include "entitycomponentui.h"


bool EntityComponentUI::newComponent(EntityComponentType *component_type)
{
  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("New");

  if (ImGui::BeginPopup("New"))
  {
    if (ImGui::MenuItem("Transform"))
      *component_type = COMPONENT_TRANSFORM;

    if (ImGui::MenuItem("Script"))
      *component_type = COMPONENT_SCRIPT;

    if (ImGui::MenuItem("Level of Detail"))
      *component_type = COMPONENT_LOD;

    ImGui::EndPopup();
  }

  return *component_type != COMPONENT_NONE;
}


void drawComponent_pointlight(GameObject *object, EntityComponent *entity_component)
{
  ImGui::PushID(object->getID());

  if (ImGui::CollapsingHeader("Pointlight"))
  {
    ImGui::Checkbox("Enabled", &entity_component->pointlight->active);

    bool shadowed = entity_component->pointlight->shadowmapped;
    ImGui::Checkbox("Shadow mapping", &entity_component->pointlight->shadowmapped);
    if (shadowed != entity_component->pointlight->shadowmapped)
      Render::ren.genDepthCubemap(&entity_component->pointlight->FBO, &entity_component->pointlight->depthCubemap);

    ImGui::ColorEdit3("ambient", (float*)&entity_component->pointlight->ambient);
    ImGui::ColorEdit3("diffuse", (float*)&entity_component->pointlight->diffuse);

    ImGui::DragFloat("radius", &entity_component->pointlight->radius, 0.1f, 0.0f, 50.0f);
    ImGui::DragFloat("constant", &entity_component->pointlight->constant, 0.1f, 0.0f, 100.0f);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &entity_component->pointlight->linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &entity_component->pointlight->quadratic, 0.001f, 0);
    ImGui::DragScalar("bias", ImGuiDataType_Float, &entity_component->pointlight->bias,            0.001f, 0);

    ImGui::Checkbox("Volumetrics", &entity_component->pointlight->volumetrics_active);
    ImGui::DragScalar("fog constant",  ImGuiDataType_Float, &entity_component->pointlight->fog_constant,    0.001f, 0);
    ImGui::DragScalar("fog linear",    ImGuiDataType_Float, &entity_component->pointlight->fog_linear,      0.001f, 0);
    ImGui::DragScalar("fog quadratic", ImGuiDataType_Float, &entity_component->pointlight->fog_quadratic,   0.001f, 0);
    ImGui::DragScalar("fog intensity", ImGuiDataType_Float, &entity_component->pointlight->fog_intensity,   0.001f, 0);
  }
  ImGui::PopID();
}


void EntityComponentUI::drawComponent_transform(GameObject *object, EntityComponent *entity_component)
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


void EntityComponentUI::drawComponent_script(GameObject *object, EntityComponent *entity_component)
{
  entity_component->script_changed = false;

  std::string label = "";
  label += ICON_FA_FILE_O;
  label += "  Script        " + entity_component->script_path.filename().string();
  label += " ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap))
  {
    if (ImGui::Button(std::string(entity_component->script_name + ".lua").c_str()))
      ImGui::OpenPopup("Change Script");

    ImGui::SetNextWindowSize({300, 300});
    if (ImGui::BeginPopup("Change Script"))
    {
      EngineUI::draw_directory_recursive(fs::current_path()/"LuaScripting/scripts", &entity_component->script_path, &entity_component->script_changed);
      
      if (entity_component->script_changed)
      {

        fs::path filepath = fs::relative(entity_component->script_path, ".");
        entity_component->script_name = filepath.string();
        entity_component->script_name.erase(entity_component->script_name.size() - 4);
      
        entity_component->script_path = filepath;


        printf("script_path: %s\n", filepath.c_str());

      }

      ImGui::EndPopup();
    }
  }
}


glm::vec3 calculate_barycentric(float x, float y, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
  float denom = (v2.y-v3.y)*(v1.x-v3.x) + (v3.x-v2.x)*(v1.y-v3.y);

  glm::vec3 weights;
  weights.x = ((v2.y-v3.y)*(x-v3.x) + (v3.x-v2.x)*(y-v3.y)) / denom;
  weights.y = ((v3.y-v1.y)*(x-v3.x) + (v1.x-v3.x)*(y-v3.y)) / denom;
  weights.z = 1 - weights.x - weights.y;
  return weights;
}

static void draw_terrain(GameObject *object, EntityComponent *terrain_component)
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

        for (size_t i=0; i<object->m_model->m_meshes[0].vertices.size(); i+=3)
        {
          Vertex v1 = object->m_model->m_meshes[0].vertices[i+0];
          Vertex v2 = object->m_model->m_meshes[0].vertices[i+1];
          Vertex v3 = object->m_model->m_meshes[0].vertices[i+2];

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


static void draw_lod(GameObject *object, EntityComponent *lod_component)
{
  std::string label = "";
  label += ICON_FA_EYE;
  label += "  Level of Detail ##" + std::to_string(object->getID());

  if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
  {
    const int max_lod = object->getLevelsLOD() - 1;
    ImGui::SliderInt("LOD", object->getLOD(), 0, max_lod);
  }

}


void EntityComponentUI::drawComponent(GameObject *object, EntityComponent *entity_component)
{
  switch (entity_component->component_type)
  {
    case (COMPONENT_NONE):
      break;


    case (COMPONENT_TRANSFORM):
      EntityComponentUI::drawComponent_transform(object, entity_component);
      break;


    case (COMPONENT_POINT_LIGHT):
      drawComponent_pointlight(object, entity_component);
      break;


    case (COMPONENT_SCRIPT):
      {
        std::vector<EntityComponent> *script_components = object->entity_components.getScriptComponents();
        
        int i = 0;
        for (EntityComponent script_component: *script_components)
        {
          ImGui::PushID(i);
          EntityComponentUI::drawComponent_script(object, &script_component);
          ImGui::PopID();
          i += 1;
        }
      }
      break;
  

    case (COMPONENT_TERRAIN):
      draw_terrain(object, entity_component);
      break;
    
    
    case (COMPONENT_LOD):
      draw_lod(object, entity_component);
      break;
  

    // case (COMPONENT_VARIABLE):
    //   this->_draw_variable(object);
    //   break;
  }

}