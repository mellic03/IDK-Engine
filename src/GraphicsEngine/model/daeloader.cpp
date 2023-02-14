#include "daeloader.h"

using namespace std;


ColladaEffect::ColladaEffect(rapidxml::xml_node<> *node)
{
  this->m_dae_id = node->first_attribute("id")->value();
  this->m_dae_id = "#" + this->m_dae_id;
  rapidxml::xml_node<> *nd = node->first_node("profile_COMMON")->first_node("newparam");
  nd = nd->first_node("surface")->first_node("init_from");

  rapidxml::xml_node<> *lambert = node->first_node("profile_COMMON")->first_node("technique")->first_node("lambert");
  this->m_specular_exponent = std::stof(lambert->first_node("index_of_refraction")->first_node("float")->value());
  if (this->m_specular_exponent < 2.0f)
    this->m_specular_exponent = 128.0f;

  this->m_image_dae_id = nd->value();
}


