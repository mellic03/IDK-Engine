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

  if (ImGui::CollapsingHeader(buffer, ImGuiTreeNodeFlags_DefaultOpen))
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

  std::string title = "Script        " + entity_component->script_path.filename().string();

  if (ImGui::CollapsingHeader(title.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap))
  {
    if (ImGui::Button(std::string(entity_component->script_name + ".lua").c_str()))
      ImGui::OpenPopup("Change Script");

    ImGui::SetNextWindowSize({300, 300});
    if (ImGui::BeginPopup("Change Script"))
    {
      bool changed = false;
      EngineUI::draw_directory_recursive(fs::current_path()/"LuaScripting/scripts", &entity_component->script_path, &entity_component->script_changed);
      
      if (entity_component->script_changed)
      {
        fs::path filepath = fs::relative(entity_component->script_path, ".");
        entity_component->script_name = filepath.string();
        entity_component->script_name.erase(entity_component->script_name.size() - 4);
      }

      ImGui::EndPopup();
    }
  }
}


void draw_terrain(GameObject *object, EntityComponent *terrain_component)
{
  if (ImGui::CollapsingHeader("Terrain"))
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

        for (size_t i=0; i<object->m_model->m_meshes[0].vertices.size(); i+=1)
        {
          Vertex vertex = object->m_model->m_meshes[0].vertices[i];

          float density = vertex.color.g;

          glm::vec3 p = vertex.position;
          
          p = model_mat * glm::vec4(p.x, p.y, p.z, 1.0f);

          for (int i=0; i<ceil(density*25.0f); i++)
          {
            float r1 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f - -1.5f)));
            float r2 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f - -1.5f)));
            Transform transform;
            transform.position = p + glm::vec3(r1, 0.0f, r2);
            World::scene.m_scenegraph->newObjectInstance("grass", transform);
            count += 1;
          }

        }


        auto data = World::scene.m_scenegraph->getInstanceData();
        InstanceData *iData = &data->at("grass");
        iData->genVBO();
        iData->setVertexAttribs();

        printf("Placed %d grass instances\n", count);

      }
    }
    //----------------------------------------------------------------


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
  

    // case (COMPONENT_VARIABLE):
    //   this->_draw_variable(object);
    //   break;
  }

}