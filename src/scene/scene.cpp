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

  this->ren->useShader(SHADER_TERRAIN);

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

  this->ren->useShader(SHADER_LIGHTSOURCE);

  int i = 0;
  for (auto &lightsource: this->m_scenegraph->m_lightsource_instances)
  {
    lightsource->m_model->setTransform(lightsource->getTransform());
    this->ren->drawLightSource(lightsource->m_model, {1, 1, 1}, 0);
    i += 1;
  }


}

