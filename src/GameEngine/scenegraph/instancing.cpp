#include "scenegraph.h"


void SceneGraph::addInstanceData(std::string template_name, Model *model, Transform *transform)
{
  this->_instance_data[template_name].addInstance(model, transform);
}


std::map<std::string, InstanceData> *SceneGraph::getInstanceData(void)
{
  return &this->_instance_data;
}


void SceneGraph::perFrameUpdate(void)
{
  for (auto it=this->_instance_data.begin(); it!=this->_instance_data.end(); ++it)
  {
    auto &data = *it;
    data.second.perFrameUpdate();
  }
}