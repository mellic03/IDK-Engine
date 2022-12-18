#include "daeloader.h"

using namespace std;

ColladaEffect::ColladaEffect(rapidxml::xml_node<> *node)
{
  rapidxml::xml_node<> *original = node;

  this->m_id = node->first_attribute()->value();

  node = node->first_node("profile_COMMON")->first_node("newparam");
  node = node->first_node("surface")->first_node("init_from");

  cout << node->value() << endl;

  this->_surface_init_from = node->value();
  

  node = original;
}

void loadLibraryImages(rapidxml::xml_node<> *node)
{
  rapidxml::xml_node<> *original = node;
  

  


  node = original;
}
