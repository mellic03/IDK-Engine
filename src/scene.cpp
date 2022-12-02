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

void Scene::addLightsourceModel(Model *lightsource_model)
{
  this->lightsource_model = lightsource_model;
}

void Scene::draw(SDL_Event *event)
{
  for (int i=0; i<this->physicsContainers.size(); i++)
    this->physicsContainers[i]->collide(this->player);

  this->renderables.objects[0]->perFrameUpdate(ren);
  this->renderables.objects[0]->collideWithMesh(this->renderables.objects[1]->model);

  this->renderables.draw(ren);

        
  for (int i=0; i<this->renderables.objects[0]->path.size(); i++)
  {
    this->lightsource_model->set_pos(this->renderables.objects[0]->path[i]);
    this->lightsource_model->draw(ren);
  }

  // this->ren->spotlights[0].position = glm::vec3(-2.0f, 4.0f, -1.0f);
  // this->ren->spotlights[0].direction = -glm::vec3(-2.0f, 4.0f, -1.0f);


  this->ren->useShader(SHADER_LIGHTSOURCE);
  for (int i=0; i<5; i++)
  {
    this->lightsource_model->materials[0].diffuse_color = this->ren->pointlights[i].diffuse;
    this->lightsource_model->set_pos(this->ren->pointlights[i].position);
    this->lightsource_model->draw(this->ren);
  }
  this->lightsource_model->set_pos(this->renderables.objects[0]->pos + this->renderables.objects[0]->ray_down);
  this->lightsource_model->draw(this->ren);

}



