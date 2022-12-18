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
    std::string m_image_dae_id = "DEFAULT";
    Texture *m_imageptr = nullptr;

    ColladaEffect(rapidxml::xml_node<> *node);

};

class ColladaMaterial {

  private:



  public:
    ColladaEffect *m_parent_effect;

    std::string m_dae_id;

    ColladaMaterial(std::string self_id, std::vector<ColladaEffect> *effects, std::string effect_id)
    {
      this->m_dae_id = self_id;

      for (auto &effect: *effects)
        if (effect.m_dae_id == effect_id)
        {
          this->m_parent_effect = &effect;
          return;
        }

      printf("Couldn't find parent effect\n");
      exit(1);

    };

};