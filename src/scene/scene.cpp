#include "scene.h"


Scene::Scene(void) { }

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


void Scene::updateLights(void)
{
  this->m_scenegraph->updateLights();
}


void Scene::sendLightsToShader(void)
{
  this->updateLights();

  this->ren->active_shader->setInt("num_active_pointlights", this->m_scenegraph->_num_active_pointlights);
  this->ren->active_shader->setInt("num_active_spotlights", this->m_scenegraph->_num_active_spotlights);

  char buffer[64];

  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->ren->depthCubemap);
    
  this->ren->active_shader->setInt("depthMap", 10);
  this->ren->active_shader->setFloat("far_plane",   25.0f);

  // Shadow mapped pointlight
  this->ren->active_shader->setVec3( "pointlight.ambient", this->m_scenegraph->pointlights[0].ambient);
  this->ren->active_shader->setVec3( "pointlight.diffuse", this->m_scenegraph->pointlights[0].diffuse);
  this->ren->active_shader->setVec3( "pointlight.pos", this->m_scenegraph->pointlights[0].m_transform->getPos_worldspace());
  this->ren->active_shader->setVec3( "pointlight.tangent_pos", this->m_scenegraph->pointlights[0].m_transform->getPos_worldspace());
  this->ren->active_shader->setFloat("pointlight.constant", this->m_scenegraph->pointlights[0].constant);
  this->ren->active_shader->setFloat("pointlight.linear", this->m_scenegraph->pointlights[0].linear);
  this->ren->active_shader->setFloat("pointlight.quadratic", this->m_scenegraph->pointlights[0].quadratic);
  this->ren->active_shader->setFloat("pointlight.bias", this->m_scenegraph->pointlights[0].bias);
  this->ren->active_shader->setVec3( "clearColor", this->ren->clearColor);
  this->ren->active_shader->setFloat("fog_start", this->ren->fog_start);
  this->ren->active_shader->setFloat("fog_end", this->ren->fog_end);



  for (int i=0; i<this->m_scenegraph->_num_active_spotlights; i++)
  {
    sprintf(buffer, "spotlights[%d].position", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "spotlights[%d].direction", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i]->m_transform->getRot());

    sprintf(buffer, "spotlights[%d].ambient", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i]->ambient);

    sprintf(buffer, "spotlights[%d].diffuse", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i]->diffuse);

    sprintf(buffer, "spotlights[%d].constant", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i]->constant);

    sprintf(buffer, "spotlights[%d].linear", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i]->linear);

    sprintf(buffer, "spotlights[%d].quadratic", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i]->quadratic);

    sprintf(buffer, "spotlights[%d].inner_cutoff", i);
    this->ren->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->m_scenegraph->sorted_spotlights[i]->inner_cutoff)));

    sprintf(buffer, "spotlights[%d].outer_cutoff", i);
    this->ren->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->m_scenegraph->sorted_spotlights[i]->outer_cutoff)));

    sprintf(buffer, "spotlights[%d].intensity", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i]->intensity);
  }

  this->ren->active_shader->setVec3("viewPos", this->ren->cam.m_transform->getPos_worldspace());
  this->ren->active_shader->setVec3("viewDirection", glm::vec3(glm::pitch(this->ren->cam.m_transform->orientation), 0.0f, 0.0f));

}


void Scene::clear(void)
{
  SceneGraph *scenegraph = this->m_scenegraph;

  this->m_scenegraph->clearScene();
}

void Scene::defaultScene(void)
{
  this->m_scenegraph->defaultScene();

  this->m_scenegraph->newObjectInstance("player");
  GameObject *obj = this->m_scenegraph->rearObjectPtr();
  this->player->useGameObject(obj);
}


void Scene::importScene(std::string filepath, Player *player)
{
  this->clear();
  this->m_scenegraph->importScene(filepath, player);
}


void Scene::drawGeometry(SDL_Event *event)
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

    if (obj.hasLightSource())
    {
      this->_lightsource_queue.push_back(&obj);
    }
  }

}


void Scene::drawLightsources(SDL_Event *event)
{
  for (auto &obj: this->_lightsource_queue)
  {
    obj->m_model->setTransform(obj->getTransform());
    this->ren->drawLightSource(obj->m_model, *obj->lightsource_components[0].diffuse);
  }

  this->_lightsource_queue.clear();
}



Scene World::scene;

