#include "entitycomponent.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../ui/UIEngine.h"

namespace fs = std::filesystem;




// void EntityComponent::_draw_spotlight(void)
// {
//   if (ImGui::CollapsingHeader("Spotlight"))
//   {
//     ImGui::Checkbox("Enable", &this->_spotlight->active);

//     ImGui::ColorEdit3("ambient", (float*)&this->_spotlight->ambient);
//     ImGui::ColorEdit3("diffuse", (float*)&this->_spotlight->diffuse);

//     ImGui::SliderFloat("constant", &this->_spotlight->constant, 0.0f, 100.0f, "%0.4f", 0);
//     ImGui::DragScalar("linear", ImGuiDataType_Float, &this->_spotlight->linear,       0.001f, 0);
//     ImGui::DragScalar("quadratic", ImGuiDataType_Float, &this->_spotlight->quadratic, 0.001f, 0);

//     ImGui::DragScalar("intensity", ImGuiDataType_Float, &this->_spotlight->intensity, 0.05f, 0);
//     ImGui::DragScalar("inner cutoff", ImGuiDataType_Float, &this->_spotlight->inner_cutoff, 0.05f, 0);
//     ImGui::DragScalar("outer cutoff", ImGuiDataType_Float, &this->_spotlight->outer_cutoff, 0.05f, 0);
//     ImGui::DragScalar("fov", ImGuiDataType_Float, &this->_spotlight->fov, 0.05f, 0);
//   }
// }



// void EntityComponent::_draw_terrain(GameObject *object)
// {
//   if (ImGui::CollapsingHeader("Terrain"))
//   {
//     ImGui::DragFloat("threshold", &this->terrain_component.threshold, 0.001f);
//     ImGui::DragFloat("epsilon",   &this->terrain_component.epsilon,   0.001f);
  
//     // High code
//     //----------------------------------------------------------------
//     static bool first = true;

//     // if (first)
//     // {
//       // if (ImGui::Button("Do the thing"))
//       // {
//       //   first = false;
        
//       //   int count = 0;

//       //   glm::mat4 model_mat = object->getTransform()->getModelMatrix();

//       //   for (size_t i=0; i<object->m_model->m_meshes[0].vertices.size(); i+=1)
//       //   {
//       //     Vertex vertex = object->m_model->m_meshes[0].vertices[i];

//       //     float density = vertex.color.g;

//       //     glm::vec3 p = vertex.position;
          
//       //     p = model_mat * glm::vec4(p.x, p.y, p.z, 1.0f);

//       //     for (int i=0; i<ceil(density*25.0f); i++)
//       //     {
//       //       float r1 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f - -1.5f)));
//       //       float r2 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f - -1.5f)));
//       //       World::scene.m_scenegraph->newObjectInstance("grass", p + glm::vec3(r1, 0.0f, r2));
//       //       count += 1;
//       //     }

//       //   }


//       //   auto data = World::scene.m_scenegraph->getInstanceData();
//       //   InstanceData *iData = &data->at("grass");
//       //   iData->genVBO();
//       //   iData->setVertexAttribs();

//       //   printf("Placed %d grass instances\n", count);

//       // }
//     // }
//     //----------------------------------------------------------------


//   }
// }



// void EntityComponent::draw(GameObject *object)
// {
//   switch (this->_component_type)
//   {
//     case (COMPONENT_NONE):
//       break;


//     case (COMPONENT_TRANSFORM):
//       this->_draw_transform(object);
//       break;


//     case (COMPONENT_LIGHTSOURCE):
//       this->_draw_lightsource();
//       break;


//     case (COMPONENT_SCRIPT):
//       this->_draw_script(object);
//       break;
  

//     case (COMPONENT_TERRAIN):
//       this->_draw_terrain(object);
//       break;
  

//     case (COMPONENT_VARIABLE):
//       this->_draw_variable(object);
//       break;
//   }

// }


void EntityComponent::toFile(std::ofstream &stream)
{
  // if (this->_pointlight != nullptr)
  // {
  //   stream << "active: " << this->_pointlight->active << "\n";
  //   stream << "shadowmapped: " << this->_pointlight->shadowmapped << "\n";
  //   stream << "radius: " << this->_pointlight->radius << "\n";
  //   glm::vec3 v;
  //   v = this->_pointlight->diffuse;
  //   stream << "diffuse: " << v.x << " " << v.y << " " << v.z << "\n";
  //   v = this->_pointlight->ambient;
  //   stream << "ambient: " << v.x << " " << v.y << " " << v.z << "\n";
  //   stream << "constant: "  << this->_pointlight->constant   << "\n";
  //   stream << "linear: "    << this->_pointlight->linear     << "\n";
  //   stream << "quadratic: " << this->_pointlight->quadratic  << "\n";
  //   stream << "bias: "      << this->_pointlight->bias       << "\n";
  // }

  // else if (this->_spotlight != nullptr)
  // {
  //   glm::vec3 v;
  //   v = this->_spotlight->diffuse;
  //   stream << "diffuse: " << v.x << " " << v.y << " " << v.z << "\n";
  //   v = this->_spotlight->ambient;
  //   stream << "ambient: " << v.x << " " << v.y << " " << v.z << "\n";
  //   stream << "constant: "  << this->_spotlight->constant   << "\n";
  //   stream << "linear: "    << this->_spotlight->linear     << "\n";
  //   stream << "quadratic: " << this->_spotlight->quadratic  << "\n";

  //   stream << "inner_cutoff: " << this->_spotlight->quadratic  << "\n";
  //   stream << "outer_cutoff: " << this->_spotlight->quadratic  << "\n";
  //   stream << "intensity: "    << this->_spotlight->quadratic  << "\n";
  // }

}


static glm::vec3 stringToVec3(std::string str)
{
  glm::vec3 v;

  std::stringstream ss;
  ss << str;

  int count = 0;
  float n;
  while (ss >> n)
  {
    v[count] = n;
    count += 1;
  }
  return v;
}

void EntityComponent::fromFile(std::ifstream &stream)
{
  std::string line;
  std::stringstream ss;

  // if (this->_pointlight != nullptr)
  // {
  //   while (getline(stream, line))
  //   {
  //     if (line.find("active: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("active: ").size());
  //       printf("active: %s\n", (line == "1" ? "true" : "false"));
  //       this->_pointlight->active == (line == "1" ? true : false);   
  //     }

  //     if (line.find("shadowmapped: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("shadowmapped: ").size());
  //       this->_pointlight->shadowmapped == (line == "1" ? true : false);
  //       if (this->_pointlight->shadowmapped == true)
  //         Render::ren.genDepthCubemap(&this->_pointlight->FBO, &this->_pointlight->depthCubemap);   
  //     }

  //     if (line.find("radius: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("radius: ").size());
  //       float f = std::stof(line);
  //       this->_pointlight->radius = f;
  //     }

  //     if (line.find("diffuse: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("diffuse: ").size());
  //       this->_pointlight->diffuse = stringToVec3(line);        
  //     }
  //     else if (line.find("ambient: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("ambient: ").size());
  //       this->_pointlight->ambient = stringToVec3(line);      
  //     }
  //     else if (line.find("constant: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("constant: ").size());
  //       float f = std::stof(line);
  //       this->_pointlight->constant = f;
  //     }
  //     else if (line.find("linear: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("linear: ").size());
  //       float f = std::stof(line);
  //       this->_pointlight->linear = f;
  //     }
  //     else if (line.find("quadratic: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("quadratic: ").size());
  //       float f = std::stof(line);
  //       this->_pointlight->quadratic = f;
  //     }
  //     else if (line.find("bias: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("bias: ").size());
  //       float f = std::stof(line);
  //       this->_pointlight->bias = f;
  //     }

  //     else if (line == "#LIGHTSOURCE END")
  //       return;
  //   }
  // }

  // else if (this->_spotlight != nullptr)
  // {
  //   while (getline(stream, line))
  //   {
  //     if (line.find("diffuse: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("diffuse: ").size());
  //       this->_pointlight->diffuse = stringToVec3(line);        
  //     }
  //     else if (line.find("ambient: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("ambient: ").size());
  //       this->_pointlight->ambient = stringToVec3(line);      
  //     }
  //     else if (line.find("constant: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("constant: ").size());
  //       ss << line;
  //       ss >> this->_pointlight->constant;
  //     }
  //     else if (line.find("linear: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("linear: ").size());
  //       ss << line;
  //       ss >> this->_pointlight->linear;
  //     }
  //     else if (line.find("quadratic: ") != std::string::npos)
  //     {
  //       line.erase(0, std::string("quadratic: ").size());
  //       ss << line;
  //       ss >> this->_pointlight->quadratic;
  //     }


  //     else if (line == "#LIGHTSOURCE END")
  //       return;
  //   }
  // }

}