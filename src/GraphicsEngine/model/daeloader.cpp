#include "daeloader.h"

using namespace std;


ColladaEffect::ColladaEffect(rapidxml::xml_node<> *node)
{
  this->m_dae_id = node->first_attribute("id")->value();
  this->m_dae_id = "#" + this->m_dae_id;
  node = node->first_node("profile_COMMON")->first_node("newparam");
  node = node->first_node("surface")->first_node("init_from");
  this->m_image_dae_id = node->value();
}


