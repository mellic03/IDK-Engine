#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../../include/rapidxml/rapidxml.hpp"

#include "texture.h"

class ColladaEffect {

  private:
    

  public:
    
    std::string m_dae_id;
    std::string m_material_dae_id;
    std::string m_image_dae_id = "DEFAULT";
    Texture *m_imageptr = nullptr;

    ColladaEffect(rapidxml::xml_node<> *node);

};

class ColladaMaterial {

  private:


  public:

    std::string m_dae_id;
    ColladaEffect *m_parent_effect;

    ColladaMaterial(std::string self_id)  { this->m_dae_id = self_id; };

};