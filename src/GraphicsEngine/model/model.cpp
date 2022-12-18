#include <fstream>
#include <sstream>

#include "model.h"

#define doit for (int i=0; i<
#define times ; i++)



std::vector<glm::vec2> parseArray_vec2(std::string string_array)
{
  std::vector<glm::vec2> vec2_array;

  std::istringstream input(string_array);

  int count = 0;
  float f;
  glm::vec2 temp;

  while (input >> f)
  {
    temp[count] = f;

    count += 1;
  
    if (count == 2)
    {
      count = 0;
      vec2_array.push_back(temp);
    }
  }

  return vec2_array;
}


std::vector<glm::vec3> parseArray_vec3(std::string string_array)
{
  std::vector<glm::vec3> vec3_array;

  std::istringstream input(string_array);

  int count = 0;
  float f;
  glm::vec3 temp;

  while (input >> f)
  {
    temp[count] = f;

    count += 1;
  
    if (count == 3)
    {
      count = 0;
      vec3_array.push_back(temp);
    }
  }

  return vec3_array;
}


void Model::loadDae(std::string filepath)
{
  std::ifstream fh;
  fh.open(filepath);

  if (fh.is_open() == false)
  {
    printf("Error opening file: %s\n", filepath.c_str());
    exit(1);
  }

  std::string raw_document = "";
  std::string line;
  while (getline(fh, line))
    raw_document += line;
    
  fh.close();


  rapidxml::xml_document<> doc;
  doc.parse<0>((char *)raw_document.c_str());

  rapidxml::xml_node<> *node = doc.first_node();


  // Images
  node = node->first_node("library_effects")->first_node("effect");

  for (rapidxml::xml_node<> *nd = node; nd != nullptr; nd = nd->next_sibling())
  {
    this->_collada_effects.push_back(ColladaEffect(nd));
  }



  node = doc.first_node()->first_node("library_geometries");
  node = node->first_node("geometry");

  for (node; node != nullptr; node = node->next_sibling())
  {
    rapidxml::xml_node<> *nd = node->first_node("mesh");


    this->m_meshes.push_back(Mesh());
    Mesh *mesh = &this->m_meshes[this->m_meshes.size() - 1];


    nd = nd->first_node("source")->first_node("float_array");
    std::vector<glm::vec3> positions = parseArray_vec3(nd->value());

    nd = nd->parent()->next_sibling()->first_node("float_array");
    std::vector<glm::vec3> normals = parseArray_vec3(nd->value());

    nd = nd->parent()->next_sibling()->first_node("float_array");
    std::vector<glm::vec2> texcoords = parseArray_vec2(nd->value());


    nd = nd->parent()->parent()->first_node("triangles");
    nd = nd->first_node("p");
    std::string string_array = nd->value();
    std::vector<int> indices;
    std::istringstream input(string_array);
    
    int i;
    while (input >> i)
      indices.push_back(i);


    for (int i=0; i<indices.size()/3; i+=1)
    {
      doit 3 times
        mesh->vertices.push_back(Vertex());

      mesh->vertices[i].position  = positions[indices[3*i + 0]];    
      mesh->vertices[i].normal    = normals  [indices[3*i + 1]];    
      mesh->vertices[i].texcoords = texcoords[indices[3*i + 2]];    
    }

    mesh->setBufferData();

  }


}




