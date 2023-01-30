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


std::vector<float> stringToFloatArray(std::string str)
{
  std::istringstream input(str);
  std::vector<float> float_array;
  float f;
  while (input >> f)
    float_array.push_back(f);

  return float_array;
}


std::vector<int> stringToIntArray(std::string str)
{
  std::istringstream input(str);
  std::vector<int> int_array;
  int i;
  while (input >> i)
    int_array.push_back(i);

  return int_array;
}


std::vector<std::string> stringToStringArray(std::string str)
{
  std::istringstream input(str);
  std::vector<std::string> str_array;
  std::string stri;
  while (input >> str)
    str_array.push_back(str);

  return str_array;
}


std::vector<glm::mat4> stringToMat4Array(std::string str)
{
  std::istringstream input(str);
  std::vector<glm::mat4> mat4_array;
  glm::mat4 mat;

  int i = 0, j = 0;
  float f;

  while (input >> f)
  {
    mat[j][i] = f;
    j+=1;

    if (j == 4)
    {
      j = 0;
      i += 1;
    }

    if (i == 4)
    {
      i = 0;
      j = 0;

      mat4_array.push_back(mat);
    }
  }

  return mat4_array;
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
  int geometry_number = 0;
  while (node != nullptr)
  {

    // For each <mesh>
    rapidxml::xml_node<> *meshNode = node->first_node("mesh");
    while (meshNode != nullptr)
    {
      this->m_meshes.push_back(Mesh());
      Mesh *mesh = &this->m_meshes[this->m_meshes.size() - 1];
      mesh->m_dae_id = "#" + std::string(node->first_attribute("id")->value());


      // For each <triangles>
      rapidxml::xml_node<> *triangleNode = meshNode->first_node("triangles");
      while (triangleNode != nullptr)
      {
        int num_triangles = std::stoi(triangleNode->first_attribute("count")->value());

        rapidxml::xml_node<> *n = triangleNode->first_node("p");

        mesh->IBOS.push_back(0);
        mesh->indices.push_back(std::vector<GLuint>());


        ColladaEffect *effectptr = this->colladaEffectPtr_materialID(triangleNode->first_attribute("material")->value());
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


        int pos_offset  = this->_geometry_position_offsets[geometry_number];
        int norm_offset = this->_geometry_normal_offsets[geometry_number];
        int tex_offset  = this->_geometry_texcoord_offsets[geometry_number];
        int col_offset  = this->_geometry_color_offsets[geometry_number];


        if (this->_colors.size() == 0)
        {
          for (size_t i=0; i<indices.size()/3; i+=1)
          {
            mesh->vertices.push_back(Vertex());
            Vertex *vertex = &mesh->vertices[mesh->vertices.size() - 1];

            vertex->position  = this->_positions [pos_offset + indices[3*i + 0]];
            vertex->normal    = this->_normals   [norm_offset + indices[3*i + 1]];
            vertex->texcoords = this->_texcoords [tex_offset + indices[3*i + 2]];

            mesh->indices[mesh->indices.size()-1].push_back(mesh->vertices.size() - 1);
          }
        }

        else
        {
          for (size_t i=0; i<indices.size()/4; i+=1)
          {
            mesh->vertices.push_back(Vertex());
            Vertex *vertex = &mesh->vertices[mesh->vertices.size() - 1];

            vertex->position  = this->_positions [pos_offset + indices[4*i + 0]];
            vertex->normal    = this->_normals   [norm_offset + indices[4*i + 1]];
            vertex->texcoords = this->_texcoords [tex_offset + indices[4*i + 2]];
            vertex->color     = this->_colors    [col_offset + indices[4*i + 3]];

            mesh->indices[mesh->indices.size()-1].push_back(mesh->vertices.size() - 1);
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
         triangleNode = triangleNode->next_sibling("triangles");
      }

      geometry_number += 1;
      meshNode = meshNode->next_sibling();
    }


    node = node->next_sibling("geometry");
  }

}


void Model::applyMeshTransforms(rapidxml::xml_document<> *doc)
{
  if (this->_animated)
    return;

  rapidxml::xml_node<> *node = doc->first_node();
  node = node->first_node("library_visual_scenes")->first_node("visual_scene");
  node = node->first_node("node");

  while (node != nullptr)
  {
    glm::mat4 mat = parseArray_mat4(node->first_node("matrix")->value());

    if (node->first_node("instance_geometry") == nullptr)
    {
      node = node->next_sibling();
      continue;
    }

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


void Model::loadVertices(rapidxml::xml_document<> *doc)
{
  rapidxml::xml_node<> *node = doc->first_node();

  rapidxml::xml_node<> *geometryNode = node->first_node("library_geometries")->first_node("geometry");


  this->_geometry_position_offsets.push_back(0);
  this->_geometry_normal_offsets.push_back(0);
  this->_geometry_texcoord_offsets.push_back(0);
  this->_geometry_color_offsets.push_back(0);


  // Load vertices
  //--------------------------------------------------------------------

  node = doc->first_node();
  geometryNode = node->first_node("library_geometries")->first_node("geometry");

  while (geometryNode != nullptr)
  {
    rapidxml::xml_node<> *meshNode = geometryNode->first_node("mesh");

    while (meshNode != nullptr)
    {
      rapidxml::xml_node<> *positionsNode = meshNode->first_node("source");
      rapidxml::xml_node<> *normalsNode   = positionsNode->next_sibling();
      rapidxml::xml_node<> *texCoordsNode = normalsNode->next_sibling();
      rapidxml::xml_node<> *colorsNode    = texCoordsNode->next_sibling();

      std::vector<glm::vec3> positions = parseArray_vec3(positionsNode->first_node("float_array")->value());
      std::vector<glm::vec3> normals   = parseArray_vec3(normalsNode->first_node("float_array")->value());
      std::vector<glm::vec2> texCoords = parseArray_vec2(texCoordsNode->first_node("float_array")->value());

      std::vector<glm::vec4> colors;
      if (colorsNode != nullptr)
        if (colorsNode->first_attribute("name") != nullptr)
          colors = parseArray_vec4(colorsNode->first_node("float_array")->value());


      for (size_t i=0; i<positions.size(); i+=1)
        this->_positions.push_back(positions[i]);
      this->_geometry_position_offsets.push_back(positions.size());

      for (size_t i=0; i<normals.size(); i+=1)
        this->_normals.push_back(normals[i]);
      this->_geometry_normal_offsets.push_back(normals.size());

      for (size_t i=0; i<texCoords.size(); i+=1)
        this->_texcoords.push_back(texCoords[i]);
      this->_geometry_texcoord_offsets.push_back(texCoords.size());

      for (size_t i=0; i<colors.size(); i+=1)
        this->_colors.push_back(colors[i]);
      this->_geometry_color_offsets.push_back(colors.size());


      this->_num_meshes += 1;
      meshNode = meshNode->next_sibling("mesh");
    }

    this->_num_geometries += 1;
    geometryNode = geometryNode->next_sibling("geometry");
  }
  //--------------------------------------------------------------------

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





void Model::loadAnimations(rapidxml::xml_document<> *doc, Animation::Armature *armature)
{
  if (this->_animated == false)
    return;

  rapidxml::xml_node<> *node = doc->first_node();

  node = node->first_node("library_animations");

  rapidxml::xml_node<> *animationNode = node->first_node("animation")->first_node("animation");

  while (animationNode != nullptr)
  {
    std::string source_bone = animationNode->first_node("source")->first_attribute("id")->value();
    
    source_bone.erase(0, armature->name.size() + 1);

    size_t pos = source_bone.find("_");
    if (pos != std::string::npos)
      source_bone.erase(0, pos + 1);
  
    pos = source_bone.find("_pose_matrix-input");
    if (pos != std::string::npos)
      source_bone.erase(pos, std::string("_pose_matrix-input").size());



    //-------------------------------------------------------------------------
    // The std::string "source_bone" is the name of the bone which
    // the weights under library_animations->animation->animation apply to.
    Animation::Joint *joint = armature->find(source_bone);
    if (joint != nullptr)
    {
      std::vector<float> keyframe_times = stringToFloatArray(animationNode->first_node("source")->first_node("float_array")->value());

      rapidxml::xml_node<> *matrixNode = animationNode->first_node("source")->next_sibling("source");
      std::vector<glm::mat4> keyframe_matrices = stringToMat4Array(matrixNode->first_node("float_array")->value());  
      
      joint->keyframe_times = keyframe_times;
      joint->keyframe_matrices = keyframe_matrices;
    }

    else
    {
      printf("[Model::loadAnimations()] Could not find joint: %s\n", source_bone.c_str());
      exit(1);
    }
    //-------------------------------------------------------------------------


    animationNode = animationNode->next_sibling("animation");
  }

}


static void recurse_loadArmature(rapidxml::xml_node<> *node, Animation::Armature *armature, Animation::Joint *joint)
{
  if (node == nullptr)
    return;

  rapidxml::xml_node<> *nd = node->first_node("node");

  // For each child joint
  while (nd != nullptr)
  {
    std::string id   = nd->first_attribute("id")->value();
    std::string name = nd->first_attribute("name")->value();
    std::string type = nd->first_attribute("type")->value();
    glm::mat4 transform = parseArray_mat4(nd->first_node("matrix")->value());

    Animation::Joint *child_joint = new Animation::Joint(id, name, type, transform);
    child_joint->_id = armature->joints.size();
    child_joint->parent = joint;
    joint->children.push_back(child_joint);
    armature->joints.push_back(child_joint);

    recurse_loadArmature(nd, armature, child_joint); 

    nd = nd->next_sibling("node");
  }
}


void Model::loadArmature(rapidxml::xml_document<> *doc, Animation::Armature *armature)
{
  rapidxml::xml_node<> *node = doc->first_node();

  if (node->first_node("library_animations") == nullptr)
  {
    printf("[Model::loadArmature()] Can't find library_animations: %s\n", this->m_name.c_str());
    exit(1);
  }

  this->_animated = true;

  // Load armature hierachy
  //--------------------------------------------------------------------------
  node = node->first_node("library_visual_scenes");
  node = node->first_node("visual_scene")->first_node("node");
  
  armature->name = node->first_attribute("id")->value();
  printf("armature name: %s\n", armature->name.c_str());

  // node = node->first_node("node");


  std::string id   = node->first_attribute("id")->value();
  std::string name = node->first_attribute("name")->value();
  std::string type = node->first_attribute("type")->value();
  glm::mat4 bindTransform = parseArray_mat4(node->first_node("matrix")->value());

  armature->root = new Animation::Joint(id, name, type, bindTransform);
  armature->root->_id = armature->joints.size();
  armature->joints.push_back(armature->root);

  rapidxml::xml_node<> *nd = node;
  while (nd != nullptr)
  {
    recurse_loadArmature(nd, armature, armature->root);
    nd = nd->next_sibling("node");
  }

  //--------------------------------------------------------------------------
}


static void printBVTree(const std::string &prefix, const Animation::Joint *node)
{
  if (node == nullptr)
    return;
  
  std::cout << prefix;
  std::cout << "    ";
  std::cout << node->_name_str << "  ID: " << node->_id << "\n";


  for (Animation::Joint *child: node->children)
    printBVTree(prefix + "    ", child);

}


void Model::loadArmatureWeights(rapidxml::xml_document<> *doc, Animation::Armature *armature)
{
  if (this->_animated == false)
    return;

  rapidxml::xml_node<> *node = doc->first_node();

  // Load joint-weight information
  //--------------------------------------------------------------------------
  node = node->first_node("library_controllers");
  node = node->first_node("controller")->first_node("skin");


  rapidxml::xml_node<> *skinNode = node;

  rapidxml::xml_node<> *jointIDNode = skinNode->first_node("source");
  std::vector<std::string> jointnames = stringToStringArray(jointIDNode->first_node("Name_array")->value());


  rapidxml::xml_node<> *bindNode = jointIDNode->next_sibling();
  std::vector<glm::mat4> bind_matrices = stringToMat4Array(bindNode->first_node("float_array")->value());

  for (size_t i=0; i<bind_matrices.size(); i++)
  {
    auto *joint = armature->find(jointnames[i]);
    joint->inverseBindTransform = bind_matrices[i];
    joint->is_animated = true;
  }


  rapidxml::xml_node<> *weightsNode = jointIDNode->next_sibling()->next_sibling();
  std::vector<float> weights = stringToFloatArray(weightsNode->first_node("float_array")->value());
  

  rapidxml::xml_node<> *vertexWeightsNode = node->first_node("vertex_weights");
  std::vector<int> vcount = stringToIntArray(vertexWeightsNode->first_node("vcount")->value());
  std::vector<int> v      = stringToIntArray(vertexWeightsNode->first_node("v")->value());


  for (size_t i=0; i<vcount.size(); i++)
  {
    this->_jointIDs.push_back(std::vector<int>());
    this->_jointWeights.push_back(std::vector<float>());
  }

  size_t cursor = 0;
  for (size_t i=0; i<vcount.size(); i++)
  {
    size_t v_end = 2*vcount[i];

    for (size_t j=cursor; j<cursor+v_end; j+=2)
    {
      this->_jointIDs[i].push_back(armature->find(jointnames[v[j + 0]])->_id);
      this->_jointWeights[i].push_back(weights[v[j + 1]]);
    }

    for (Mesh &mesh: this->m_meshes)
    {
      for (Vertex &vertex: mesh.vertices)
      {
        if (vertex.position == this->_positions[i])
        {
          std::vector<int>   jids;
          std::vector<float> jweights;

          for (size_t j=cursor, k=0; j<cursor+v_end; j+=2, k+=1)
          {
            jids.push_back(armature->find(jointnames[v[j + 0]])->_id);         
            jweights.push_back(weights[v[j + 1]]);
          }


          if (jids.size() > 4)
          {
            // Sort weights
            for (size_t k=0; k<jids.size(); k++)
            {
              for (size_t m=0; m<k; m++)
              {
                if (k != m)
                {
                  if (jweights[k] > jweights[m])
                  {
                    int tempint = jids[k];
                    jids[k] = jids[m];
                    jids[m] = tempint;

                    float tempfloat = jweights[k];
                    jweights[k] = jweights[m];
                    jweights[m] = tempfloat;
                  }
                }
              }
            }

            for (size_t j=0; j<4; j++)
            {
              vertex.joint_ids[j] = jids[j];
              vertex.weights[j]   = jweights[j];
            }
          }

          else
          {
            for (size_t j=0; j<jids.size(); j++)
            {
              vertex.joint_ids[j] = jids[j];
              vertex.weights[j]   = jweights[j];
            }
          }
        }
      }
    }

    cursor += v_end;
  }

  //--------------------------------------------------------------------------


  // Normalize weights
  for (Mesh &mesh: this->m_meshes)
  {
    for (Vertex &vertex: mesh.vertices)
    {
      float sum = vertex.weights[0] + vertex.weights[1] + vertex.weights[2] + vertex.weights[3];
      vertex.weights /= sum;
    }
  }

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

  this->loadVertices(&doc);

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




Animation::Animation *Model::getAnimation(std::string name)
{
  return this->_animation_controller.getAnimation(name);
}


Animation::AnimationController *Model::getAnimController()
{
  return &this->_animation_controller;
}




void Model::loadAnimation(std::string name, Animation::AnimationController *animationController, std::string directory, std::string filename)
{
  this->_animated = true;

  std::ifstream fh;
  fh.open(directory + filename);
  if (fh.is_open() == false)
  {
    printf("[Model::loadDae()] Error opening file: %s\n", filename.c_str());
    exit(1);
  }

  std::string raw_document = "";
  std::string line;
  while (getline(fh, line))
    raw_document += line;
    
  fh.close();


  rapidxml::xml_document<> doc;
  doc.parse<0>((char *)raw_document.c_str());



  Animation::Animation *animation = animationController->newAnimation(name);
  Animation::Armature *armature = animation->getArmature();

  this->loadArmature(&doc, armature);
  this->loadArmatureWeights(&doc, armature);
  this->loadAnimations(&doc, armature);



  float largest = 0.0f;
  for (Animation::Joint *joint: armature->joints)
  {
    while (joint->keyframe_finalBoneTransforms.size() < joint->keyframe_matrices.size())
      joint->keyframe_finalBoneTransforms.push_back(glm::mat4(1.0f));

    for (float f: joint->keyframe_times)
    {
      if (f > largest)
        largest = f;
    }
  }
  animation->setLength(largest);


  for (Mesh &mesh: this->m_meshes)
    mesh.setBufferData();
}
