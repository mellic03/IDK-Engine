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


ColladaEffect *Model::colladaEffectPtr_materialID(std::string dae_id)
{
  for (auto &effect: this->_collada_effects)
    if (effect.m_material_dae_id == dae_id)
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




      ColladaEffect *effectptr = this->colladaEffectPtr_materialID(nd->first_attribute("material")->value());

      mesh->materials.push_back(Material());
      mesh->materials[mesh->materials.size() - 1].diffuseMap  = *this->texturePtr(effectptr->m_image_dae_id);
      mesh->materials[mesh->materials.size() - 1].specularMap = *this->texturePtr(effectptr->m_image_dae_id + "-specular");
      mesh->materials[mesh->materials.size() - 1].emissionMap = *this->texturePtr(effectptr->m_image_dae_id + "-emission");
      mesh->materials[mesh->materials.size() - 1].normalMap   = *this->texturePtr(effectptr->m_image_dae_id + "-normal");



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

      // Calculate vertex tangents
      //----------------------------------------------------
      for (int i=0; i<mesh->vertices.size(); i+=3)
      {
        Vertex *v1 = &mesh->vertices[i+0];
        Vertex *v2 = &mesh->vertices[i+1];
        Vertex *v3 = &mesh->vertices[i+2];

        glm::vec3 p1 = v1->position,  p2 = v2->position,  p3 = v3->position;
        glm::vec2 t1 = v1->texcoords, t2 = v2->texcoords, t3 = v3->texcoords;

        glm::vec3 edge1 = p2 - p1;
        glm::vec3 edge2 = p3 - p1;
        glm::vec2 deltaUV1 = t2 - t1;
        glm::vec2 deltaUV2 = t3 - t1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent = {
          f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
          f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
          f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        };

        v1->tangent = tangent;
        v2->tangent = tangent;
        v3->tangent = tangent;
      }
      //----------------------------------------------------


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

  while (node != nullptr)
  {
    std::string self_id = node->first_attribute("id")->value();
    std::string effect_id = node->first_node("instance_effect")->first_attribute()->value();


    for (auto &effect: this->_collada_effects)
    {
      if (effect.m_dae_id == effect_id)
      {
        effect.m_material_dae_id = self_id;
        break;
      }
    }



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


void Model::computeBoundingSphere(void)
{

  glm::vec3 total = glm::vec3(0.0f);
  int count = 0;
  for (auto &mesh: this->m_meshes)
  {
    for (auto &vertex: mesh.vertices)
    {
      total += vertex.position;
      count += 1;
    }
  }
  this->bounding_sphere_pos = total / (float)count;


  this->bounding_sphere_radius2 = 0.0f;
  for (auto &mesh: this->m_meshes)
  {
    for (auto &vertex: mesh.vertices)
    {
      float dist2 = glm::distance(glm::vec3(0.0f), vertex.position);
      if (dist2 > this->bounding_sphere_radius2)
        this->bounding_sphere_radius2 = dist2;
    }
  }

  printf("name: %s\n", this->m_name.c_str());
  printf("radius: %f\n\n", this->bounding_sphere_radius2);

}


void Model::loadDae(std::string directory, std::string filename, int id)
{
  std::ifstream fh;
  fh.open(directory + filename);

  if (fh.is_open() == false)
  {
    printf("Error opening file: %s\n", filename.c_str());
    exit(1);
  }

  this->m_ID = id;
  this->m_name = directory + filename;
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

  this->computeBoundingSphere();

  for (auto &mesh: this->m_meshes)
    mesh.setBufferData();


}

