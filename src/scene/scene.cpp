#include "scene.h"


void Scene::useRenderer(Renderer *renptr)
{
  this->ren = renptr;
}

void Scene::usePlayer(Player *playerptr)
{
  this->player = playerptr;
}

void Scene::useSceneGraph(SceneGraph *scenegraph)
{
  this->m_scenegraph = scenegraph;
}


void Scene::draw(SDL_Event *event)
{

  for (auto &obj: this->m_scenegraph->m_object_instances)
  {
    if (obj.isHidden())
      continue;

    for (auto &obj2: this->m_scenegraph->m_object_instances)
      obj.collideWithObject(&obj2);

    obj.perFrameUpdate(this->ren);

    if (obj.hasGeometry())
    {
      obj.m_model->setTransform(obj.getTransform());
      this->ren->drawModel(obj.m_model);
    }
  }


  Shader *backupshader = this->ren->active_shader;
  this->ren->useShader(SHADER_LIGHTSOURCE);
  for (int i=0; i<this->ren->pointlights.size(); i++)
  {
    this->_lightsource_model->setTransform(&this->m_temptransform);
    *this->_lightsource_model->getPos() = this->ren->pointlights[i].position;
    this->ren->drawLightSource(this->_lightsource_model, this->ren->pointlights[i].diffuse, i);
  }
  this->ren->active_shader = backupshader;

}

