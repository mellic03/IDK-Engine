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


glm::mat4 parseArray_mat4(std::string string_array)
{
  std::istringstream input(string_array);

  int count = 0;
  float f;
  float mat[16];

  while (input >> f)
    mat[count++] = f;

  return glm::transpose(glm::make_mat4(mat));
}


Mesh *Model::meshPtr(std::string dae_id)
{
  for (auto &mesh: this->m_meshes)
    if (mesh.m_dae_id == dae_id)
      return &mesh;

  return nullptr;
}


Texture *Model::texturePtr(std::string dae_id)
{

  for (auto &texture: this->_collada_images)
  {
    if (texture.m_dae_id == dae_id)
      return &texture;
  }

  return nullptr;
}


Texture *Model::texturePtr_materialID(std::string material_dae_id)
{
  ColladaMaterial *collada_material = this->colladaMaterialPtr(material_dae_id);
  if (collada_material == nullptr)
  {
    printf("Could not find ColladaMaterial, id: %s\n", material_dae_id.c_str());
    exit(1);
  }

  ColladaEffect *collada_effect = collada_material->m_parent_effect;
  if (collada_effect == nullptr)
  {
    printf("Could not find ColladaEffect\n");
    exit(1);
  }
}


ColladaMaterial *Model::colladaMaterialPtr(std::string dae_id)
{
  for (auto &material: this->_collada_materials)
    if (material.m_dae_id == dae_id)
      return &material;

  return nullptr;
}


ColladaEffect *Model::colladaEffectPtr(std::string dae_id)
{
  for (auto &effect: this->_collada_effects)
    if (effect.m_dae_id == dae_id)
      return &effect;

  return nullptr;
}


void Model::constructMeshes(rapidxml::xml_document<> *doc)
{
  rapidxml::xml_node<> *node = doc->first_node()->first_node("library_geometries");
  node = node->first_node("geometry");


  // For each "geometry"
  for (node; node != nullptr; node = node->next_sibling())
  {
    rapidxml::xml_node<> *nd = node->first_node("mesh");


    this->m_meshes.push_back(Mesh());
    Mesh *mesh = &this->m_meshes[this->m_meshes.size() - 1];
    mesh->m_dae_id = "#" + std::string(node->first_attribute("id")->value());


    nd = nd->first_node("source")->first_node("float_array");
    std::vector<glm::vec3> positions = parseArray_vec3(nd->value());

    nd = nd->parent()->next_sibling()->first_node("float_array");
    std::vector<glm::vec3> normals = parseArray_vec3(nd->value());

    nd = nd->parent()->next_sibling()->first_node("float_array");
    std::vector<glm::vec2> texcoords = parseArray_vec2(nd->value());


    // For each "triangles"
    nd = nd->parent()->parent()->first_node("triangles");
    while (nd != nullptr)
    {
      rapidxml::xml_node<> *n = nd->first_node("p");

      mesh->IBOS.push_back(0);
      mesh->indices.push_back(std::vector<GLuint>());







      ColladaMaterial *matptr = this->colladaMaterialPtr(nd->first_attribute("material")->value());

      Texture *textureptr = matptr->m_parent_effect->m_imageptr;


      mesh->materials.push_back(Material());
      mesh->materials[mesh->materials.size() - 1].diffuseMap = textureptr;
      // std::cout << nd->first_attribute("material")->value() << std::endl;








      std::string string_array = n->value();
      std::vector<int> indices;
      std::istringstream input(string_array);

      int i;
      while (input >> i)
        indices.push_back(i);


      for (int i=0; i<indices.size()/3; i+=1)
      {
        mesh->vertices.push_back(Vertex());
        Vertex *vertex = &mesh->vertices[mesh->vertices.size() - 1];

        vertex->position  = positions[indices[3*i + 0]];
        vertex->normal    = normals  [indices[3*i + 1]];
        vertex->texcoords = texcoords[indices[3*i + 2]];

        mesh->indices[mesh->indices.size() - 1].push_back(mesh->vertices.size() - 1);
      }

      nd = nd->next_sibling();
    }
  }

}


void Model::applyMeshTransforms(rapidxml::xml_document<> *doc)
{
  rapidxml::xml_node<> *node = doc->first_node();
  node = node->first_node("library_visual_scenes")->first_node("visual_scene");
  node = node->first_node("node");

  while (node != nullptr)
  {
    glm::mat4 mat = parseArray_mat4(node->first_node("matrix")->value());

    std::string id = node->first_node("instance_geometry")->first_attribute("url")->value();
    Mesh *mesh = this->meshPtr(id);
    mesh->transformByMatrix(mat);

    node = node->next_sibling();
  }

}


void Model::loadLibraryImages(rapidxml::xml_document<> *doc)
{
  rapidxml::xml_node<> *node = doc->first_node();
  node = node->first_node("library_images")->first_node("image");

  auto &wee = this->_collada_images;

  while (node != nullptr)
  {
    std::string filepath = node->first_node("init_from")->value();
    filepath = this->_directory + filepath;
    std::string id = node->first_attribute("id")->value();

    this->_collada_images.push_back(Texture(filepath, id, true));

    int insert_point = filepath.size() - 4;
    std::string temp_path = filepath;

    temp_path.insert(insert_point, "-specular");
    this->_collada_images.push_back(Texture(temp_path, id + "-specular", false));

    temp_path = filepath; temp_path.insert(insert_point, "-emission");
    this->_collada_images.push_back(Texture(temp_path, id + "-emission", false));

    temp_path = filepath; temp_path.insert(insert_point, "-normal");
    this->_collada_images.push_back(Texture(temp_path, id + "-normal", false));

    node = node->next_sibling();
  }
}


void Model::loadLibraryEffects(rapidxml::xml_document<> *doc)
{
  rapidxml::xml_node<> *node = doc->first_node();
  
  node = node->first_node("library_effects")->first_node("effect");

  auto &wee = this->_collada_images;

  while (node != nullptr)
  {
    rapidxml::xml_node<> *tempnode = node;

    this->_collada_effects.push_back(ColladaEffect(tempnode));
    
    std::string image_id = this->_collada_effects[this->_collada_effects.size() - 1].m_image_dae_id;
    Texture *textureptr = this->texturePtr(image_id);

    this->_collada_effects[this->_collada_effects.size() - 1].m_imageptr = textureptr;

    node = node->next_sibling();
  }

}


void Model::loadLibraryMaterials(rapidxml::xml_document<> *doc)
{
  rapidxml::xml_node<> *node = doc->first_node();
  
  node = node->first_node("library_materials")->first_node("material");

  auto &wee = this->_collada_images;

  while (node != nullptr)
  {
    std::string effect_id = node->first_node("instance_effect")->first_attribute()->value();
    std::string self_id = node->first_attribute("id")->value();
    this->_collada_materials.push_back(ColladaMaterial(self_id, &this->_collada_effects, effect_id));
    node = node->next_sibling();
  }

}



int Model::colladaImageIndex(std::string dae_id)
{
  for (int i=0; i<this->_collada_images.size(); i++)
    if (this->_collada_images[i].m_dae_id == dae_id)
      return i;

  return -1;
}


void Model::loadDae(std::string directory, std::string filename)
{
  std::ifstream fh;
  fh.open(directory + filename);

  if (fh.is_open() == false)
  {
    printf("Error opening file: %s\n", filename.c_str());
    exit(1);
  }

  this->_directory = directory;

  std::string raw_document = "";
  std::string line;
  while (getline(fh, line))
    raw_document += line;
    
  fh.close();


  rapidxml::xml_document<> doc;
  doc.parse<0>((char *)raw_document.c_str());

  rapidxml::xml_node<> *node = doc.first_node();

  this->loadLibraryImages(&doc);
  this->loadLibraryEffects(&doc);
  this->loadLibraryMaterials(&doc);

  this->constructMeshes(&doc);
  this->applyMeshTransforms(&doc);


  for (auto &mesh: this->m_meshes)
    mesh.setBufferData();


}

