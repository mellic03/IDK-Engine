#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include <fstream>

#include "daeloader.h"
#include "mesh.h"
#include "animation.h"
#include "../../transform.h"


enum ModelType {
  MODEL_TERRAIN,
  MODEL_BILLBOARD,
  MODEL_STATIC,
  MODEL_LIGHTSOURCE,
};


class Model {

  private:

    std::vector<glm::vec3> _vertex_positions;

    std::string _directory;

    std::vector<Texture> _collada_images;
    std::vector<ColladaEffect> _collada_effects;
    std::vector<ColladaMaterial> _collada_materials;

    Mesh *meshPtr(std::string dae_id);
    Texture *texturePtr(std::string dae_id);
    ColladaMaterial *colladaMaterialPtr(std::string dae_id);
    ColladaEffect *colladaEffectPtr(std::string dae_id);
    ColladaEffect *colladaEffectPtr_materialID(std::string dae_id);

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

};


