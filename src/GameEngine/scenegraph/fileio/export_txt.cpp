#include "../scenegraph.h"
#include "fileutil.h"


static void objectToFile(std::ofstream &stream, GameObject *object)
{
  stream << "<GAMEOBJECT>" << "\n";

  std::string indentation = "  ";

  FileUtil::ToText::objectheader(stream, object, indentation);
  FileUtil::ToText::transform(stream, object->getTransform(), indentation);
  FileUtil::ToText::scripts(stream, object, indentation);
  
  if (object->entity_components.hasComponent(COMPONENT_POINT_LIGHT))
    FileUtil::ToText::pointlight(stream, object->entity_components.getComponent(COMPONENT_POINT_LIGHT)->pointlight, indentation);


  // for (size_t i=0; i<object->script_components.size(); i++)
  // {
  //   *stream << "#SCRIPT: ";
  //   *stream << object->script_components[i].script_name << "\n";
  // }

  // if (object->getObjectType() == GAMEOBJECT_LIGHTSOURCE)
  // {
  //   *stream << "#LIGHTSOURCE BEGIN" << "\n";
  //   object->lightsource_components[0].toFile(*stream);
  //   *stream << "#LIGHTSOURCE END" << "\n";
  // }


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