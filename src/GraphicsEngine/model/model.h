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
#include "../transform/transform.h"


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

    std::vector<glm::ivec4>  _jointIDs;
    std::vector<glm::vec4>   _jointWeights;


    bool _animated = false;
    bool _armature_loaded = false;

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
    void loadArmature(rapidxml::xml_document<> *doc, Animation::Armature *armature);
    void loadArmatureWeights(rapidxml::xml_document<> *doc, Animation::Armature *armature);
    void loadAnimations(rapidxml::xml_document<> *doc, Animation::Armature *armature);



    void constructMeshes(rapidxml::xml_document<> *doc);
    void applyMeshTransforms(rapidxml::xml_document<> *doc);
    void loadLibraryImages(rapidxml::xml_document<> *doc);
    void loadLibraryEffects(rapidxml::xml_document<> *doc);
    void loadLibraryMaterials(rapidxml::xml_document<> *doc);
    int colladaImageIndex(std::string dae_id);

    void computeBoundingSphere(void);
    void loadBoundingSphere(std::ifstream &stream);

    void _mergeMeshes();
    void _toFile(std::string filepath);
    void _fromFile(std::string filepath);


  public:
    std::vector<Mesh> _tempMeshes;

    glm::vec3 bounding_sphere_pos  = glm::vec3(0.0f);
    float bounding_sphere_radius   = 0.0f;
    float bounding_sphere_radiusSQ = 0.0f;


    int m_ID = 0;
    std::string m_name = "DEFAULT";

    bool is_terrain = false;

    Mesh mesh;
    std::vector<Material> materials;


    void loadDae(std::string directory, std::string filename, bool instanced);
    void loadAnimation(std::string name, Animation::AnimationController *animationController, std::string directory, std::string filename);


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

    bool isEmpty();

    void loadLOD(int lod, std::string directory, std::string filename, bool is_terrain);

    std::vector<float> *getLODDistances();

    Model *getLOD_model(int lod);
    Model *getLOD_model_override(int lod);


    Model *getDefaultLOD_model();
    Model *getMaxLOD_model();
    Model *getMinLOD_model();
    Model *getShadowLOD_model();

    int    getLevelsLOD();
    int   *getDefaultLOD_value();
    int   *getMaxLOD_value();
    int   *getShadowLOD_value();

    void loadAnimationLOD(int lod, std::string animation_name, Animation::AnimationController *animationController, std::string directory, std::string filename);

};