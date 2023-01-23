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

std::vector<glm::vec4> parseArray_vec4(std::string string_array)
{
  std::vector<glm::vec4> vec4_array;

  std::istringstream input(string_array);

  int count = 0;
  float f;
  glm::vec4 temp;

  while (input >> f)
  {
    temp[count] = f;

    count += 1;
  
    if (count == 4)
    {
      count = 0;
      vec4_array.push_back(temp);
    }
  }

  return vec4_array;
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


    std::vector<glm::vec4> vertcolors;


    if (this->is_terrain)
    {
      nd = nd->parent()->next_sibling()->first_node("float_array");
      vertcolors = parseArray_vec4(nd->value());
    }


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

      if (!this->is_terrain)
      {
        for (size_t i=0; i<indices.size()/3; i+=1)
        {
          mesh->vertices.push_back(Vertex());
          Vertex *vertex = &mesh->vertices[mesh->vertices.size() - 1];

          vertex->position  = positions[indices[3*i + 0]];
          vertex->normal    = normals  [indices[3*i + 1]];
          vertex->texcoords = texcoords[indices[3*i + 2]];

          mesh->indices[mesh->indices.size() - 1].push_back(mesh->vertices.size() - 1);
        }
      }

      else
      {
        for (size_t i=0; i<indices.size()/4; i+=1)
        {
          mesh->vertices.push_back(Vertex());
          Vertex *vertex = &mesh->vertices[mesh->vertices.size() - 1];

          vertex->position  = positions [indices[4*i + 0]];
          vertex->normal    = normals   [indices[4*i + 1]];
          vertex->texcoords = texcoords [indices[4*i + 2]];
          vertex->color     = vertcolors[indices[4*i + 3]];

          mesh->indices[mesh->indices.size() - 1].push_back(mesh->vertices.size() - 1);
        }
      }


      // Calculate vertex tangents
      //----------------------------------------------------
      for (size_t i=0; i<mesh->vertices.size(); i+=3)
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
  for (size_t i=0; i<this->_collada_images.size(); i++)
    if (this->_collada_images[i].m_dae_id == dae_id)
      return i;

  return -1;
}


void Model::computeBoundingSphere(void)
{
  // Find the two vertices which are furthest from each other,
  // the initial bounding sphere center is their midpoint and
  // the radius is the distance between them divided by two.
  float max_dist_sq = 0.0f;
  glm::vec3 max_dist_v1 = glm::vec3(0.0f);
  glm::vec3 max_dist_v2 = glm::vec3(0.0f);

  for (Mesh mesh: this->m_meshes)
  {
    for (auto &v1: mesh.vertices)
    {
      for (auto &v2: mesh.vertices)
      {
        float distSQ = glm::distance2(v1.position, v2.position);
        if (distSQ > max_dist_sq)
        {
          max_dist_sq = distSQ;
          max_dist_v1 = v1.position;
          max_dist_v2 = v2.position;
        }
      }
    }
  }

  
  this->bounding_sphere_pos = (max_dist_v1 + max_dist_v2) / 2.0f;
  this->bounding_sphere_radiusSQ = (sqrt(max_dist_sq) / 2.0f) * (sqrt(max_dist_sq) / 2.0f);


  // Loop over vertices again, if any are outside the sphere,
  // increase the sphere size by the amount the vertex is outside
  for (Mesh mesh: this->m_meshes)
  {
    for (auto &v: mesh.vertices)
    {
      float dist = glm::distance2(v.position, this->bounding_sphere_pos);
      
      if (dist > this->bounding_sphere_radiusSQ)
        this->bounding_sphere_radiusSQ += (dist - bounding_sphere_radiusSQ);
    }
  }

  this->bounding_sphere_radius = sqrt(this->bounding_sphere_radiusSQ);
}

void Model::loadBoundingSphere(std::ifstream &stream)
{
  std::string line;

  getline(stream, line);
  std::istringstream input(line);
  for (int i=0; i<3; i++)
    input >> this->bounding_sphere_pos[i];

  getline(stream, line);
  this->bounding_sphere_radius = std::stof(line);

  this->bounding_sphere_radiusSQ = this->bounding_sphere_radius * this->bounding_sphere_radius;
}


void Model::loadDae(std::string directory, std::string filename, bool is_terrain)
{
  std::ifstream fh;
  fh.open(directory + filename);

  if (fh.is_open() == false)
  {
    printf("[Model::loadDae()] Error opening file: %s\n", filename.c_str());
    exit(1);
  }

  this->m_ID = 0;
  this->m_name = directory + filename;
  this->_directory = directory;

  if (is_terrain)
    this->is_terrain = true;

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


  if (this->is_terrain)
  {
    node = doc.first_node();
    node = node->first_node("library_visual_scenes");
    node = node->first_node("visual_scene");
    node = node->first_node("node");
    node = node->first_node("instance_geometry");
    node = node->first_node("bind_material");
    node = node->first_node("technique_common");
    node = node->first_node("instance_material");

    for (int i=0; i<3; i++)
    {
      std::string name = node->first_attribute("symbol")->value();

      ColladaEffect *effectptr = this->colladaEffectPtr_materialID(name);

      this->materials.push_back(Material());
      this->materials[this->materials.size() - 1].diffuseMap  = *this->texturePtr(effectptr->m_image_dae_id);
      this->materials[this->materials.size() - 1].specularMap = *this->texturePtr(effectptr->m_image_dae_id + "-specular");
      this->materials[this->materials.size() - 1].emissionMap = *this->texturePtr(effectptr->m_image_dae_id + "-emission");
      this->materials[this->materials.size() - 1].normalMap   = *this->texturePtr(effectptr->m_image_dae_id + "-normal");

      node = node->next_sibling();
    }
  }

  this->constructMeshes(&doc);
  this->applyMeshTransforms(&doc);


  std::ifstream istream(directory + "boundingsphere.txt");
  if (istream.good())
    this->loadBoundingSphere(istream);

  else
  {
    this->computeBoundingSphere();
    std::ofstream ostream(directory + "boundingsphere.txt");
    ostream << this->bounding_sphere_pos.x << " " << this->bounding_sphere_pos.y << " " << this->bounding_sphere_pos.z << "\n";
    ostream << this->bounding_sphere_radius << std::endl;
    ostream.close();
  }

  istream.close();


  for (auto &mesh: this->m_meshes)
    mesh.setBufferData();

}

