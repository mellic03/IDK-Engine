#include "scene.h"


Scene::Scene(void)
{
  this->pointlights_on[0] = true;
  this->pointlights[0].diffuse = { 1.0f, 1.0f, 1.0f };
}

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
  this->sorted_pointlights.clear();
  this->sorted_spotlights.clear();

  this->_num_active_pointlights = 0;
  this->_num_active_spotlights = 0;

  int count = 0;

  for (int i=0; i<NUM_POINTLIGHTS; i++)
    if (this->pointlights_on[i])
    {
      this->sorted_pointlights.push_back(this->pointlights[i]);
      this->_num_active_pointlights += 1;
    }
  
  for (int i=0; i<NUM_POINTLIGHTS; i++)
    if (!this->pointlights_on[i])
      this->sorted_pointlights.push_back(this->pointlights[i]);
  

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    if (this->spotlights_on[i])
    {
      this->sorted_spotlights.push_back(this->spotlights[i]);
      this->_num_active_spotlights += 1;
    }

  for (int i=0; i<NUM_SPOTLIGHTS; i++)
    if (!this->spotlights_on[i])
      this->sorted_spotlights.push_back(this->spotlights[i]);
  
}

void Scene::sendLightsToShader(void)
{
  this->ren->active_shader->setInt("num_active_pointlights", this->_num_active_pointlights);
  this->ren->active_shader->setInt("num_active_spotlights", this->_num_active_spotlights);

  char buffer[64];

  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->ren->depthCubemap);
    
  this->ren->active_shader->setInt("depthMap", 10);
  this->ren->active_shader->setFloat("far_plane",   25.0f);

  this->ren->active_shader->setVec3( "pointlight.ambient", this->pointlights[0].ambient);
  this->ren->active_shader->setVec3( "pointlight.diffuse", this->pointlights[0].diffuse);
  this->ren->active_shader->setVec3( "pointlight.pos", this->pointlights[0].m_transform->getPos_worldspace());
  this->ren->active_shader->setVec3( "pointlight.tangent_pos", this->pointlights[0].m_transform->getPos_worldspace());
  this->ren->active_shader->setFloat("pointlight.constant", this->pointlights[0].constant);
  this->ren->active_shader->setFloat("pointlight.linear", this->pointlights[0].linear);
  this->ren->active_shader->setFloat("pointlight.quadratic", this->pointlights[0].quadratic);
  this->ren->active_shader->setFloat("pointlight.bias", this->pointlights[0].bias);
  this->ren->active_shader->setVec3( "clearColor", this->ren->clearColor);
  this->ren->active_shader->setFloat("fog_start", this->ren->fog_start);
  this->ren->active_shader->setFloat("fog_end", this->ren->fog_end);



  for (int i=0; i<NUM_SPOTLIGHTS; i++)
  {
    sprintf(buffer, "spotlights[%d].position", i);
    this->ren->active_shader->setVec3(buffer,  this->sorted_spotlights[i].position);

    sprintf(buffer, "spotlights[%d].direction", i);
    this->ren->active_shader->setVec3(buffer,  this->sorted_spotlights[i].direction);

    sprintf(buffer, "spotlights[%d].ambient", i);
    this->ren->active_shader->setVec3(buffer,  this->sorted_spotlights[i].ambient);

    sprintf(buffer, "spotlights[%d].diffuse", i);
    this->ren->active_shader->setVec3(buffer,  this->sorted_spotlights[i].diffuse);

    sprintf(buffer, "spotlights[%d].specular", i);
    this->ren->active_shader->setVec3(buffer,  this->sorted_spotlights[i].specular);

    sprintf(buffer, "spotlights[%d].constant", i);
    this->ren->active_shader->setFloat(buffer,  this->sorted_spotlights[i].constant);

    sprintf(buffer, "spotlights[%d].linear", i);
    this->ren->active_shader->setFloat(buffer,  this->sorted_spotlights[i].linear);

    sprintf(buffer, "spotlights[%d].quadratic", i);
    this->ren->active_shader->setFloat(buffer,  this->sorted_spotlights[i].quadratic);

    sprintf(buffer, "spotlights[%d].inner_cutoff", i);
    this->ren->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->sorted_spotlights[i].inner_cutoff)));

    sprintf(buffer, "spotlights[%d].outer_cutoff", i);
    this->ren->active_shader->setFloat(buffer,  glm::cos(glm::radians(this->sorted_spotlights[i].outer_cutoff)));

    sprintf(buffer, "spotlights[%d].intensity", i);
    this->ren->active_shader->setFloat(buffer,  this->sorted_spotlights[i].intensity);
  }

  this->ren->active_shader->setVec3("viewPos", this->ren->cam.m_transform->getPos_worldspace());
  this->ren->active_shader->setVec3("viewDirection", glm::mat3(this->ren->cam.modifier_matrix) * *this->ren->cam.dir);

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
  }

}


void Scene::drawLightsources(SDL_Event *event)
{
  int i = 0;
  for (auto &lightsource: this->m_scenegraph->m_lightsource_instances)
  {
    if (this->pointlights_on[0] == false)
      continue;
    this->pointlights[0].m_transform = lightsource->getTransform();
    lightsource->m_model->setTransform(lightsource->getTransform());
    this->ren->drawLightSource(lightsource->m_model, this->pointlights[0].diffuse);
    i += 1;
  }
}




Scene World::scene;


