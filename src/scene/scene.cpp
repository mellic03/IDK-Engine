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



  for (int i=0; i<NUM_SPOTLIGHTS; i++)
  {
    sprintf(buffer, "spotlights[%d].position", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i].m_transform->getPos_worldspace());

    sprintf(buffer, "spotlights[%d].direction", i);
    this->ren->active_shader->setVec3(buffer,  {0.0f, 0.0f, 0.0f});

    sprintf(buffer, "spotlights[%d].ambient", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i].ambient);

    sprintf(buffer, "spotlights[%d].diffuse", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i].diffuse);

    sprintf(buffer, "spotlights[%d].specular", i);
    this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_spotlights[i].specular);

    sprintf(buffer, "spotlights[%d].constant", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i].constant);

    sprintf(buffer, "spotlights[%d].linear", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i].linear);

    sprintf(buffer, "spotlights[%d].quadratic", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i].quadratic);

    sprintf(buffer, "spotlights[%d].inner_cutoff", i);
    this->ren->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->m_scenegraph->sorted_spotlights[i].inner_cutoff)));

    sprintf(buffer, "spotlights[%d].outer_cutoff", i);
    this->ren->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->m_scenegraph->sorted_spotlights[i].outer_cutoff)));

    sprintf(buffer, "spotlights[%d].intensity", i);
    this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_spotlights[i].intensity);
  }

  this->ren->active_shader->setVec3("viewPos", this->ren->cam.m_transform->getPos_worldspace());
  this->ren->active_shader->setVec3("viewDirection", glm::vec3(glm::pitch(this->ren->cam.m_transform->orientation), 0.0f, 0.0f));

}


void Scene::clear(void)
{
  this->m_scenegraph->clearScene();

  SceneGraph *scenegraph = this->m_scenegraph;

  scenegraph->newObjectInstance("empty");
  this->player->m_gameobject = scenegraph->rearObjectPtr();
  this->player->m_gameobject->setName("Player");
  this->player->m_gameobject->setInteractivity("player");
  this->player->cam->useTransform(this->player->getTransform());




  // scenegraph->newObjectInstance("pointlight");
  // GameObject *light1 = scenegraph->rearObjectPtr();
  // light1->setName("Point light 0");
  // light1->lightsource_components.push_back(EntityComponent(COMPONENT_LIGHTSOURCE, &this->m_scenegraph->pointlights[0]));
  // light1->hasGeometry(false);
  // this->m_scenegraph->pointlights[0].m_transform = light1->getTransform();
  // scenegraph->m_pointlight_instances.push_back(light1);

  // pointlights->giveChild(light1);


  // for (int i=0; i<NUM_SPOTLIGHTS; i++)
  // {
  //   scenegraph->newObjectInstance("spotlight");
  //   GameObject *spotlight = scenegraph->rearObjectPtr();
  //   spotlight->setName("Spot light " + std::to_string(i));
  //   spotlight->lightsource_components.push_back(EntityComponent(COMPONENT_LIGHTSOURCE, &this->spotlights[i]));
  //   spotlight->hasGeometry(false);
  //   this->spotlights[i].m_transform = spotlight->getTransform();
  //   scenegraph->m_spotlight_instances.push_back(spotlight);
  //   spotlights->giveChild(spotlight);
  // }

}


void Scene::importScene(std::string filepath)
{
  this->clear();
  this->m_scenegraph->importScene(filepath);
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

