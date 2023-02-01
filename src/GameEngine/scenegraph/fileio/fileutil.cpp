#include "../scenegraph.h"
#include "fileutil.h"



void FileUtil::ToText::objectheader(std::ofstream &stream, GameObject *object, std::string indentation)
{
  stream << indentation << "<HEADER>\n";

  stream << indentation + "  " << "template name: " << object->getTemplateName() << "\n";
  stream << indentation + "  " << "assigned name: " << object->getName() << "\n";
  stream << indentation + "  " << "objectID: " << object->getID() << "\n";
  stream << indentation + "  " << "parentID: " << object->parentID << "\n";
  stream << indentation + "  " << "GameObjectFlags: " << static_cast<GLuint>(*object->getData()->getFlags()) << "\n";

  stream << indentation << "</HEADER>" << std::endl;
}


void FileUtil::ToText::transform(std::ofstream &stream, Transform *transf, std::string indentation)
{
  stream << indentation << "<TRANSFORM>\n";


  glm::vec3 v;
  v = transf->getPos_worldspace();
  stream << indentation + "  " << "position: " << v.x << " " << v.y << " " << v.z << "\n";

  v = *transf->getVel();
  stream << indentation + "  " << "velocity: " << v.x << " " << v.y << " " << v.z << "\n";

  glm::quat q = transf->orientation;
  stream << indentation + "  " << "orientation: " << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << "\n";

  v = *transf->getScale();
  stream << indentation + "  " << "scale: " << v.x << " " << v.y << " " << v.z << "\n";

  stream << indentation << "</TRANSFORM>" << std::endl;
}


void FileUtil::ToText::scripts(std::ofstream &stream, GameObject *object, std::string indentation)
{
  stream << indentation << "<SCRIPTS>\n";

  std::vector<ScriptComponent> *script_components = object->getComponents()->getScriptComponents();
  for (ScriptComponent script_component: *script_components)
    stream << indentation + "  " << "script: " << script_component.script_name << "\n";
  
  stream << indentation << "</SCRIPTS>" << std::endl;
} 


void FileUtil::ToText::pointlight(std::ofstream &stream, PointLight *pointlight, std::string indentation)
{
  if (pointlight == nullptr)
    return;

  stream << indentation << "<POINTLIGHT>\n";
  
    stream << indentation + "  " << "active: " << pointlight->active << "\n";
    stream << indentation + "  " << "shadowmapped: " << pointlight->shadowmapped << "\n";
    stream << indentation + "  " << "volumetrics: "  << pointlight->volumetrics_active << "\n";
    stream << indentation + "  " << "radius: " << pointlight->radius << "\n";
    glm::vec3 v;
    v = pointlight->diffuse;
    stream << indentation + "  " << "diffuse: " << v.x << " " << v.y << " " << v.z << "\n";
    v = pointlight->ambient;
    stream << indentation + "  " << "ambient: " << v.x << " " << v.y << " " << v.z << "\n";
    stream << indentation + "  " << "constant: "  << pointlight->constant   << "\n";
    stream << indentation + "  " << "linear: "    << pointlight->linear     << "\n";
    stream << indentation + "  " << "quadratic: " << pointlight->quadratic  << "\n";
    stream << indentation + "  " << "bias: "      << pointlight->bias       << "\n";

  stream << indentation << "</POINTLIGHT>" << std::endl;
}


void FileUtil::ToText::physics(std::ofstream &stream, GameObject *object, std::string indentation)
{
  if (object->getData()->getFlag(GameObjectFlag::PHYSICS) == false)
    return;

  stream << indentation << "<PHYSICS>\n";
  stream << indentation << "</PHYSICS>" << std::endl;;
}


void FileUtil::ToText::spherecollider(std::ofstream &stream, GameObject *object, std::string indentation)
{
  if (object->getComponents()->hasComponent(COMPONENT_SPHERE_COLLIDER) == false)
    return;

  stream << indentation << "<SPHERECOLLIDER>\n";
  stream << indentation + "  " << "radius: " << object->spherecollider.radius << "\n";
  stream << indentation << "</SPHERECOLLIDER>" << std::endl;
}






glm::vec3 stringToVec3(std::string stringdata)
{
  std::stringstream ss;
  ss << stringdata;
  glm::vec3 v;
  int count = 0;
  float n;
  while (ss >> n)
  {
    v[count] = n;
    count += 1;
  }
  return v;
}


static void eraseUpTo(std::string &line, std::string stopat)
{
  size_t pos = line.find(stopat);
  line.erase(0, pos + stopat.size());
}


GameObjectHeader FileUtil::FromText::objectheader(std::ifstream &stream, GameObject *object)
{
  GameObjectHeader header;
  std::string line;
  
  getline(stream, line);
  eraseUpTo(line, "template name: ");
  header.template_name = line;

  getline(stream, line);
  eraseUpTo(line, "assigned name: ");
  header.assigned_name = line;

  getline(stream, line);
  eraseUpTo(line, "objectID: ");
  header.objectID = std::stoi(line);

  getline(stream, line);
  eraseUpTo(line, "parentID: ");
  header.parentID = std::stoi(line);

  getline(stream, line);
  eraseUpTo(line, "GameObjectFlags: ");
  header.flags = static_cast<GLuint>(std::stoi(line));

  return header;
}



void FileUtil::FromText::transform(std::ifstream &stream, Transform *transf)
{
  size_t pos_start, vel_start, orient_start, scale_start;
  size_t pos_end = std::string("position: ").size();
  size_t vel_end = std::string("velocity: ").size();
  size_t scale_end = std::string("scale: ").size();
  size_t orientation_end = std::string("orientation: ").size();
  

  std::string line;
  while (getline(stream, line))
  {
    pos_start = line.find("position: ");
    vel_start = line.find("velocity: ");
    scale_start = line.find("scale: ");
    orient_start = line.find("orientation: ");

    if (pos_start != std::string::npos)
    {
      line.erase(0, pos_start + pos_end);
      *transf->getPos() = stringToVec3(line);
    }

    else if (vel_start != std::string::npos)
    {
      line.erase(0, vel_start + vel_end);
      *transf->getVel() = stringToVec3(line);
    }

    else if (scale_start != std::string::npos)
    {
      line.erase(0, scale_start + scale_end);
      *transf->getScale() = stringToVec3(line);
    }

    else if (orient_start != std::string::npos)
    {
      line.erase(0, orient_start + orientation_end);
      std::stringstream ss;
      ss << line;
      glm::quat q;
      int count = 0;
      float n;
      while (ss >> n)
      {
        q[count] = n;
        count += 1;
      }

      transf->orientation = q;
    }

    else if (line.find("</TRANSFORM>") != std::string::npos)
      return;
  }
}


void FileUtil::FromText::scripts(std::ifstream &stream, GameObject *object)
{
  std::string line;
  size_t script_start;

  while (getline(stream, line))
  {
    script_start = line.find("script: ");

    if (script_start != std::string::npos)
    {
      line.erase(0, script_start + std::string("script: ").size());
      object->getComponents()->giveComponent(ScriptComponent(line));
    }

    else if (line.find("</SCRIPTS>") != std::string::npos)
      return;
  }
}


void FileUtil::FromText::pointlight(std::ifstream &stream, PointLight *pointlight)
{
  std::string line;

  getline(stream, line);
  line.erase(0, line.find("active: ") + std::string("active: ").size());
  pointlight->active = std::stoi(line);

  getline(stream, line);
  line.erase(0, line.find("shadowmapped: ") + std::string("shadowmapped: ").size());
  pointlight->shadowmapped = std::stoi(line);

  getline(stream, line);
  line.erase(0, line.find("volumetrics: ") + std::string("volumetrics: ").size());
  pointlight->volumetrics_active = std::stoi(line);

  getline(stream, line);
  line.erase(0, line.find("radius: ") + std::string("radius: ").size());
  pointlight->radius = std::stof(line);

  getline(stream, line);
  line.erase(0, line.find("diffuse: ") + std::string("diffuse: ").size());
  pointlight->diffuse = stringToVec3(line);

  getline(stream, line);
  line.erase(0, line.find("ambient: ") + std::string("ambient: ").size());
  pointlight->ambient = stringToVec3(line);

  getline(stream, line);
  line.erase(0, line.find("constant: ") + std::string("constant: ").size());
  pointlight->constant = std::stof(line);

  getline(stream, line);
  line.erase(0, line.find("linear: ") + std::string("linear: ").size());
  pointlight->linear = std::stof(line);

  getline(stream, line);
  line.erase(0, line.find("quadratic: ") + std::string("quadratic: ").size());
  pointlight->quadratic = std::stof(line);

  getline(stream, line);
  line.erase(0, line.find("bias: ") + std::string("bias: ").size());
  pointlight->bias = std::stof(line);

}


void FileUtil::FromText::spherecollider(std::ifstream &stream, GameObject *object)
{
  std::string line;

  getline(stream, line);
  eraseUpTo(line, "radius: ");
  object->spherecollider.radius = std::stof(line);

}



