#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glu.h>

#include <fstream>

#include "daeloader.h"
#include "mesh.h"
#include "../../transform.h"


enum ModelType {
  MODEL_TERRAIN,
  MODEL_BILLBOARD,
  MODEL_STATIC,
  MODEL_LIGHTSOURCE,
};


class Model {

  private:
    std::vector<int> _geometry_position_offsets;
    std::vector<int> _geometry_normal_offsets;
    std::vector<int> _geometry_texcoord_offsets;
    std::vector<int> _geometry_color_offsets;
    
    std::vector<glm::vec3> _positions;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec2> _texcoords;
    std::vector<glm::vec4> _colors;

    std::vector<int>    _mesh_vertex_offsets;
    std::vector<Vertex *> _vertices;

    Animation::Armature _armature;

    bool _animated = false;
    int _num_geometries = 0;
    int _num_meshes = 0;

    std::string _directory;

    std::vector<Texture> _collada_images;
    std::vector<ColladaEffect> _collada_effects;
    std::vector<ColladaMaterial> _collada_materials;

    Mesh *meshPtr(std::string dae_id);
    Texture *texturePtr(std::string dae_id);
    ColladaMaterial *colladaMaterialPtr(std::string dae_id);
    ColladaEffect *colladaEffectPtr(std::string dae_id);
    ColladaEffect *colladaEffectPtr_materialID(std::string dae_id);
  

    void loadVertices(rapidxml::xml_document<> *doc);
    void loadArmature(rapidxml::xml_document<> *doc);
    void loadArmatureWeights(rapidxml::xml_document<> *doc);
    void loadAnimations(rapidxml::xml_document<> *doc);
    void constructMeshes(rapidxml::xml_document<> *doc);
    void applyMeshTransforms(rapidxml::xml_document<> *doc);
    void loadLibraryImages(rapidxml::xml_document<> *doc);
    void loadLibraryEffects(rapidxml::xml_document<> *doc);
    void loadLibraryMaterials(rapidxml::xml_document<> *doc);
    int colladaImageIndex(std::string dae_id);

    void computeBoundingSphere(void);
    void loadBoundingSphere(std::ifstream &stream);

  public:

    glm::vec3 bounding_sphere_pos  = glm::vec3(0.0f);
    float bounding_sphere_radius   = 0.0f;
    float bounding_sphere_radiusSQ = 0.0f;


    int m_ID = 0;
    std::string m_name = "DEFAULT";

    bool is_terrain = false;

    std::vector<Mesh> m_meshes;
    std::vector<Material> materials;


    void loadDae(std::string directory, std::string filename, bool instanced);
    bool isAnimated() { return this->_animated; };
};



struct ModelLOD {

  private:  
    bool _first_loaded = false;
    std::vector<Model> _models;
    std::vector<float> _lod_distances;
    int _default_lod = 0;
    int _max_lod = this->_default_lod;
    int _shadow_lod = this->_default_lod;

  public:

    bool isEmpty()  { return !this->_first_loaded; };

    void loadLOD(int lod, std::string directory, std::string filename, bool is_terrain)
    {
      while (this->_models.size() <= static_cast<size_t>(lod))
      {
        this->_models.push_back(Model());
        this->_lod_distances.push_back(0.0f);
      }
      
      this->_models[lod].loadDae(directory, filename, is_terrain);
      this->_first_loaded = true;
    }; 

    std::vector<float> *getLODDistances() { return &this->_lod_distances; };

    Model *getLOD_model(int lod)
    {
      if (lod >= this->_max_lod)
        return &this->_models[lod];
      else 
        return &this->_models[this->_max_lod];
    };
    Model *getLOD_model_override(int lod)  { return &this->_models[lod]; };


    Model *getDefaultLOD_model()    { return &this->_models[this->_default_lod]; };
    Model *getMaxLOD_model()        { return &this->_models[this->_max_lod];     };
    Model *getMinLOD_model()        { return &this->_models[this->_models.size() - 1]; };
    Model *getShadowLOD_model()     { return &this->_models[this->_shadow_lod];  };

    int    getLevelsLOD()           { return this->_models.size(); };
    int   *getDefaultLOD_value()    { return &this->_default_lod;  };
    int   *getMaxLOD_value()        { return &this->_max_lod;      };
    int   *getShadowLOD_value()     { return &this->_shadow_lod;   };
};