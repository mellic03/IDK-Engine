#include "scene.h"


void Scene::useRenderer(Renderer *renptr)
{
  this->ren = renptr;
}

void Scene::usePlayer(Player *playerptr)
{
  this->player = playerptr;
}

void Scene::addLightsourceModel(Mesh *lightsource_model)
{
  this->lightsource_model = lightsource_model;
}

void Scene::draw(SDL_Event *event)
{

  for (auto &obj1: this->object_handler->m_object_instances)
  {
    if (obj1.isHidden())
      continue;
    
    for (auto &obj2: this->object_handler->m_object_instances)
      obj1.collideWithObject(&obj2);

    obj1.perFrameUpdate(this->ren);
    obj1.draw(this->ren);
    this->player->collideWith(&obj1);
  }


  this->player->perFrameUpdate();

  // this->ren->useShader(SHADER_LIGHTSOURCE);
  // for (int i=0; i<5; i++)
  // {
  //   this->lightsource_model->materials[0].diffuse_color = this->ren->pointlights[i].diffuse;
  //   this->lightsource_model->set_pos(this->ren->pointlights[i].position);
  //   this->lightsource_model->draw(this->ren);
  // }
  // this->lightsource_model->set_pos(this->renderables.objects[0]->pos + this->renderables.objects[0]->ray_down);
  // this->lightsource_model->draw(this->ren);

}

