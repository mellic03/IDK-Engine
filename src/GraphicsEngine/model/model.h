#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "daeloader.h"
#include "mesh.h"
#include "animation.h"
#include "../../transform.h"

class Model {

  private:
    std::string _directory;

    std::vector<Texture> _collada_images;
    std::vector<ColladaEffect> _collada_effects;
    std::vector<ColladaMaterial> _collada_materials;

    Mesh *meshPtr(std::string dae_id);
    Texture *texturePtr(std::string dae_id);
    Texture *texturePtr_materialID(std::string material_dae_id);
    ColladaMaterial *colladaMaterialPtr(std::string dae_id);
    ColladaEffect *colladaEffectPtr(std::string dae_id);
    ColladaEffect *colladaEffectPtr_materialID(std::string dae_id);

    void constructMeshes(rapidxml::xml_document<> *doc);
    void applyMeshTransforms(rapidxml::xml_document<> *doc);
    void loadLibraryImages(rapidxml::xml_document<> *doc);
    void loadLibraryEffects(rapidxml::xml_document<> *doc);
    void loadLibraryMaterials(rapidxml::xml_document<> *doc);
    int colladaImageIndex(std::string dae_id);

    Transform *_transform;

  public:
    int m_ID = 0;
    std::string m_name = "DEFAULT";

    std::vector<Mesh> m_meshes;

    void loadDae(std::string directory, std::string filename, int id = 0);

    void setTransform(Transform *transform) { this->_transform = transform; };

    Transform *getTransform(void) { return this->_transform; };
    glm::vec3 *getPos(void)       { return this->_transform->getPos(); };

};


