#include "model.h"




bool ModelLOD::isEmpty()  { return !this->_first_loaded; };

void ModelLOD::loadLOD(int lod, std::string directory, std::string filename, bool is_terrain)
{
  while (this->_models.size() <= static_cast<size_t>(lod))
  {
    this->_models.push_back(Model());
    this->_lod_distances.push_back(0.0f);
  }
  
  this->_models[lod].loadDae(directory, filename, is_terrain);
  this->_first_loaded = true;
}; 


std::vector<float> *ModelLOD::getLODDistances() { return &this->_lod_distances; };

Model *ModelLOD::getLOD_model(int lod)
{
  if (lod >= this->_max_lod)
    return &this->_models[lod];
  else 
    return &this->_models[this->_max_lod];
};
Model *ModelLOD::getLOD_model_override(int lod)  { return &this->_models[lod]; };


Model *ModelLOD::getDefaultLOD_model()    { return &this->_models[this->_default_lod]; };
Model *ModelLOD::getMaxLOD_model()        { return &this->_models[this->_max_lod];     };
Model *ModelLOD::getMinLOD_model()        { return &this->_models[this->_models.size() - 1]; };
Model *ModelLOD::getShadowLOD_model()     { return &this->_models[this->_shadow_lod];  };

int    ModelLOD::getLevelsLOD()           { return this->_models.size(); };
int   *ModelLOD::getDefaultLOD_value()    { return &this->_default_lod;  };
int   *ModelLOD::getMaxLOD_value()        { return &this->_max_lod;      };
int   *ModelLOD::getShadowLOD_value()     { return &this->_shadow_lod;   };



void ModelLOD::loadAnimationLOD(int lod, std::string animation_name, std::string directory, std::string filename)
{
  this->_models[lod].loadAnimation(animation_name, directory, filename);
}

