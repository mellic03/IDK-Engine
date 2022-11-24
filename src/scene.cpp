#include "scene.h"



void Scene::bindRenderer(Renderer *renptr)
{
  this->ren = renptr;
}

void Scene::bindPlayer(Player *playerptr)
{
  this->player = playerptr;
}

void Scene::addRenderContainer(ModelContainer *render_container)
{
  this->renderContainers.push_back(render_container);
}

void Scene::addPhysicsContainer(ModelContainer *physics_container)
{
  this->physicsContainers.push_back(physics_container);
}


void Scene::draw(SDL_Event *event)
{
  for (int i=0; i<this->renderContainers.size(); i++)
    this->renderContainers[i]->draw(this->ren);

  for (int i=0; i<this->physicsContainers.size(); i++)
    this->physicsContainers[i]->collide(this->player);

  this->ren->spotlights[0].position = *this->player->pos;
  this->ren->spotlights[0].direction = this->ren->cam.front;

  for (int i=0; i<NUM_DIRLIGHTS; i++)
  {
    // sphere.set_pos(this->ren->dirlights[i].direction);
    // sphere.material.diffuse_color = this->ren->dirlights[i].diffuse;
    // sphere.draw(&ren);
  }

  for (int i=0; i<NUM_POINTLIGHTS; i++)
  {
    // sphere.set_pos(this->ren->pointlights[i].position);
    // sphere.material.diffuse_color = this->ren->pointlights[i].diffuse;
    // sphere.draw(&ren);
  }

}



