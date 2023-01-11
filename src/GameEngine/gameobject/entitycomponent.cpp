#include "entitycomponent.h"
#include "../../GraphicsEngine/GraphicsEngine.h"
#include "../../ui/UIEngine.h"

namespace fs = std::filesystem;


EntityComponent::EntityComponent(EntityComponentType component_type)
{
  this->_component_type = component_type;
}

EntityComponent::EntityComponent(EntityComponentType component_type, PointLight *pointlight)
{
  this->_component_type = component_type;
  this->_pointlight = pointlight;
  this->diffuse = &pointlight->diffuse;
}

EntityComponent::EntityComponent(EntityComponentType component_type, SpotLight *spotlight)
{
  this->_component_type = component_type;
  this->_spotlight = spotlight;
  this->diffuse = &spotlight->diffuse;
}


void EntityComponent::_draw_pointlight(void)
{
  if (ImGui::CollapsingHeader("Pointlight"))
  {
    ImGui::Checkbox("Enabled", &this->_pointlight->active);

    bool shadowed = this->_pointlight->shadowmapped;
    ImGui::Checkbox("Shadow mapping", &this->_pointlight->shadowmapped);
    if (shadowed != this->_pointlight->shadowmapped)
      Render::ren.genDepthCubemap(&this->_pointlight->FBO, &this->_pointlight->depthCubemap);

    ImGui::ColorEdit3("ambient", (float*)&this->_pointlight->ambient);
    ImGui::ColorEdit3("diffuse", (float*)&this->_pointlight->diffuse);

    ImGui::DragFloat("radius", &this->_pointlight->radius, 0.1f, 0.0f, 50.0f);
    ImGui::DragFloat("constant", &this->_pointlight->constant, 0.1f, 0.0f, 100.0f);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &this->_pointlight->linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &this->_pointlight->quadratic, 0.001f, 0);
    ImGui::DragScalar("bias", ImGuiDataType_Float, &this->_pointlight->bias,            0.001f, 0);

    ImGui::DragScalar("fog constant",  ImGuiDataType_Float, &this->_pointlight->fog_constant,    0.001f, 0);
    ImGui::DragScalar("fog linear",    ImGuiDataType_Float, &this->_pointlight->fog_linear,      0.001f, 0);
    ImGui::DragScalar("fog quadratic", ImGuiDataType_Float, &this->_pointlight->fog_quadratic,   0.001f, 0);
    ImGui::DragScalar("fog intensity", ImGuiDataType_Float, &this->_pointlight->fog_intensity,   0.001f, 0);
  }
}


void EntityComponent::_draw_spotlight(void)
{
  if (ImGui::CollapsingHeader("Spotlight"))
  {
    ImGui::Checkbox("Enable", &this->_spotlight->active);

    ImGui::ColorEdit3("ambient", (float*)&this->_spotlight->ambient);
    ImGui::ColorEdit3("diffuse", (float*)&this->_spotlight->diffuse);

    ImGui::SliderFloat("constant", &this->_spotlight->constant, 0.0f, 100.0f, "%0.4f", 0);
    ImGui::DragScalar("linear", ImGuiDataType_Float, &this->_spotlight->linear,       0.001f, 0);
    ImGui::DragScalar("quadratic", ImGuiDataType_Float, &this->_spotlight->quadratic, 0.001f, 0);

    ImGui::DragScalar("intensity", ImGuiDataType_Float, &this->_spotlight->intensity, 0.05f, 0);
    ImGui::DragScalar("inner cutoff", ImGuiDataType_Float, &this->_spotlight->inner_cutoff, 0.05f, 0);
    ImGui::DragScalar("outer cutoff", ImGuiDataType_Float, &this->_spotlight->outer_cutoff, 0.05f, 0);
    ImGui::DragScalar("fov", ImGuiDataType_Float, &this->_spotlight->fov, 0.05f, 0);
  }
}


void EntityComponent::_draw_lightsource(void)
{
  if (this->_pointlight != nullptr)
    this->_draw_pointlight();
  else if (this->_spotlight != nullptr)
    this->_draw_spotlight();
}


void EntityComponent::_draw_transform(GameObject *object)
{
  char buffer[128];
  sprintf(buffer, "Transform ##%d", object->getID());
  if (ImGui::CollapsingHeader(buffer))
  {
    EngineUI::vec3("Position", object->getPos(), 0.01f);
    EngineUI::vec3("Velocity", object->getVel(), 0.01f);

    glm::quat q = object->getTransform()->orientation;
    glm::vec3 drot1 = glm::degrees(glm::eulerAngles(q));
    glm::vec3 drot2 = drot1;
    EngineUI::vec3("Rotation", &drot1, 0.1f);
    object->getTransform()->addRot(glm::radians(drot1 - drot2));
  }
}


void EntityComponent::_draw_script(GameObject *object)
{
  this->script_changed = false;
  
  std::string title = "Script        " + this->script_path.filename().string();

  if (ImGui::CollapsingHeader(title.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap))
  {
    if (ImGui::Button(std::string(this->script_name + ".lua").c_str()))
      ImGui::OpenPopup("Change Script");

    ImGui::SetNextWindowSize({300, 300});
    if (ImGui::BeginPopup("Change Script"))
    {
      bool changed = false;
      EngineUI::draw_directory_recursive(fs::current_path()/"LuaScripting/scripts", &this->script_path, &this->script_changed);
      
      if (this->script_changed)
      {
        fs::path filepath = fs::relative(this->script_path, ".");
        this->script_name = filepath.string();
        this->script_name.erase(this->script_name.size() - 4);
      }

      ImGui::EndPopup();
    }
  }
}


void EntityComponent::_draw_variable(GameObject *object)
{

}


void EntityComponent::draw(GameObject *object)
{
  switch (this->_component_type)
  {
    case (COMPONENT_NONE):
      break;

    case (COMPONENT_TRANSFORM):
      this->_draw_transform(object);
      break;


    case (COMPONENT_LIGHTSOURCE):
      this->_draw_lightsource();
      break;


    case (COMPONENT_SCRIPT):
      this->_draw_script(object);
      break;
  

    case (COMPONENT_VARIABLE):
      this->_draw_variable(object);
      break;
  }

}


void EntityComponent::toFile(std::ofstream &stream)
{
  if (this->_pointlight != nullptr)
  {
    stream << "active: " << this->_pointlight->active << "\n";
    stream << "shadowmapped: " << this->_pointlight->shadowmapped << "\n";
    stream << "radius: " << this->_pointlight->radius << "\n";
    glm::vec3 v;
    v = this->_pointlight->diffuse;
    stream << "diffuse: " << v.x << " " << v.y << " " << v.z << "\n";
    v = this->_pointlight->ambient;
    stream << "ambient: " << v.x << " " << v.y << " " << v.z << "\n";
    stream << "constant: "  << this->_pointlight->constant   << "\n";
    stream << "linear: "    << this->_pointlight->linear     << "\n";
    stream << "quadratic: " << this->_pointlight->quadratic  << "\n";
    stream << "bias: "      << this->_pointlight->bias       << "\n";
  }

  else if (this->_spotlight != nullptr)
  {
    glm::vec3 v;
    v = this->_spotlight->diffuse;
    stream << "diffuse: " << v.x << " " << v.y << " " << v.z << "\n";
    v = this->_spotlight->ambient;
    stream << "ambient: " << v.x << " " << v.y << " " << v.z << "\n";
    stream << "constant: "  << this->_spotlight->constant   << "\n";
    stream << "linear: "    << this->_spotlight->linear     << "\n";
    stream << "quadratic: " << this->_spotlight->quadratic  << "\n";

    stream << "inner_cutoff: " << this->_spotlight->quadratic  << "\n";
    stream << "outer_cutoff: " << this->_spotlight->quadratic  << "\n";
    stream << "intensity: "    << this->_spotlight->quadratic  << "\n";
  }

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

  if (this->_pointlight != nullptr)
  {
    while (getline(stream, line))
    {
      if (line.find("active: ") != std::string::npos)
      {
        line.erase(0, std::string("active: ").size());
        printf("active: %s\n", (line == "1" ? "true" : "false"));
        this->_pointlight->active == (line == "1" ? true : false);   
      }

      if (line.find("shadowmapped: ") != std::string::npos)
      {
        line.erase(0, std::string("shadowmapped: ").size());
        this->_pointlight->shadowmapped == (line == "1" ? true : false);
        if (this->_pointlight->shadowmapped == true)
          Render::ren.genDepthCubemap(&this->_pointlight->FBO, &this->_pointlight->depthCubemap);   
      }

      if (line.find("radius: ") != std::string::npos)
      {
        line.erase(0, std::string("radius: ").size());
        float f = std::stof(line);
        this->_pointlight->radius = f;
      }

      if (line.find("diffuse: ") != std::string::npos)
      {
        line.erase(0, std::string("diffuse: ").size());
        this->_pointlight->diffuse = stringToVec3(line);        
      }
      else if (line.find("ambient: ") != std::string::npos)
      {
        line.erase(0, std::string("ambient: ").size());
        this->_pointlight->ambient = stringToVec3(line);      
      }
      else if (line.find("constant: ") != std::string::npos)
      {
        line.erase(0, std::string("constant: ").size());
        float f = std::stof(line);
        this->_pointlight->constant = f;
      }
      else if (line.find("linear: ") != std::string::npos)
      {
        line.erase(0, std::string("linear: ").size());
        float f = std::stof(line);
        this->_pointlight->linear = f;
      }
      else if (line.find("quadratic: ") != std::string::npos)
      {
        line.erase(0, std::string("quadratic: ").size());
        float f = std::stof(line);
        this->_pointlight->quadratic = f;
      }
      else if (line.find("bias: ") != std::string::npos)
      {
        line.erase(0, std::string("bias: ").size());
        float f = std::stof(line);
        this->_pointlight->bias = f;
      }

      else if (line == "#LIGHTSOURCE END")
        return;
    }
  }

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