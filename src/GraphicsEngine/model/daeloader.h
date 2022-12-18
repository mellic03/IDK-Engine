#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "../../include/rapidxml/rapidxml.hpp"

class ColladaEffect {

  private:
    std::string _surface_init_from;
    

  public:
    
    std::string m_id;


    ColladaEffect(rapidxml::xml_node<> *node);

};

void loadLibraryImages(rapidxml::xml_node<> *node);