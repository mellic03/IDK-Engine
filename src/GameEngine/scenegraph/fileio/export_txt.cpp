#include "../scenegraph.h"
#include "fileutil.h"


static void objectToFile(std::ofstream &stream, GameObject *object)
{
  stream << "<GAMEOBJECT>" << "\n";

  std::string indentation = "  ";

  FileUtil::ToText::objectheader(stream, object, indentation);


  // Components
  //-----------------------------------------------------------------------
  EntityComponents *components = object->getComponents();

  FileUtil::ToText::transform(stream, object->getTransform(), indentation);
  FileUtil::ToText::scripts(stream, object, indentation);
  FileUtil::ToText::pointlight(stream, components->getPointLightComponent(), indentation);
  FileUtil::ToText::spherecollider(stream, object, indentation);
  FileUtil::ToText::physics(stream, object, indentation);
  //-----------------------------------------------------------------------




  stream << "</GAMEOBJECT>\n" << std::endl;
}



void SceneGraph::exportScene(std::string filepath)
{
  std::cout << "exporting scene to: " << filepath << "\n";

  std::ofstream stream;
  stream.open(filepath);

  stream << "<SCENE>\n";

  stream << "</SCENE>\n\n";


  for (auto &object: this->m_object_instances)
  {
    objectToFile(stream, &object);
  }

  stream.close();
}