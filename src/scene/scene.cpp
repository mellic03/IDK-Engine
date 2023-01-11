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


void Scene::sortLights(void)
{
  this->m_scenegraph->sortLights();
}


void Scene::sendLightsToShader(void)
{
  this->sortLights();

  this->ren->active_shader->setVec3( "clearColor", this->ren->clearColor);
  this->ren->active_shader->setFloat("fog_start", this->ren->fog_start);
  this->ren->active_shader->setFloat("fog_end", this->ren->fog_end);
  this->ren->active_shader->setFloat("far_plane",         25.0f );


  // Shadow mapped dirlight
  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_2D, this->ren->dirlight_depthmap);
  this->ren->active_shader->setInt(   "depthmap_dirlight", 10    );
  this->ren->active_shader->setMat4(  "dir_lightSpaceMatrix", this->ren->lightSpaceMatrix);

  this->ren->active_shader->setVec3(  "shadowmapped_dirlight.ambient", this->m_scenegraph->dirlight.ambient);
  this->ren->active_shader->setVec3(  "shadowmapped_dirlight.diffuse", this->m_scenegraph->dirlight.diffuse);
  this->ren->active_shader->setVec3(  "shadowmapped_dirlight.position", this->m_scenegraph->dirlight.position);
  this->ren->active_shader->setVec3(  "shadowmapped_dirlight.direction", this->m_scenegraph->dirlight.direction);
  this->ren->active_shader->setFloat( "shadowmapped_dirlight.bias", this->m_scenegraph->dirlight.bias);
  this->ren->active_shader->setFloat( "shadowmapped_dirlight.fog_intensity", this->m_scenegraph->dirlight.fog_intensity);

  char buffer[64];



  // Shadow mapped pointlights
  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    glActiveTexture(GL_TEXTURE11 + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_scenegraph->pointlights[i].depthCubemap);
    sprintf(buffer, "pointlights[%d].depthCubemap", i);
    this->ren->active_shader->setInt(buffer, 11 + i);


    sprintf(buffer, "pointlights[%d].is_active", i);
    this->ren->active_shader->setInt(buffer, this->m_scenegraph->pointlights[i].active);

    sprintf(buffer, "pointlights[%d].is_shadowmapped", i);
    this->ren->active_shader->setInt(buffer, this->m_scenegraph->pointlights[i].shadowmapped);

    sprintf(buffer, "pointlights[%d].radius", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].radius);

    sprintf(buffer, "pointlights[%d].ambient", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->pointlights[i].ambient);

    sprintf(buffer, "pointlights[%d].diffuse", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->pointlights[i].diffuse);

    sprintf(buffer, "pointlights[%d].position", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->pointlights[i].m_transform->getPos_worldspace());

    sprintf(buffer, "pointlights[%d].constant", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].constant);
    
    sprintf(buffer, "pointlights[%d].linear", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].linear);

    sprintf(buffer, "pointlights[%d].quadratic", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].quadratic);

    sprintf(buffer, "pointlights[%d].bias", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].bias);

    sprintf(buffer, "pointlights[%d].fog_constant", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_constant);

    sprintf(buffer, "pointlights[%d].fog_linear", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_linear);

    sprintf(buffer, "pointlights[%d].fog_quadratic", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_quadratic);

    sprintf(buffer, "pointlights[%d].fog_intensity", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_intensity);
  }



  // this->ren->active_shader->setInt("num_active_nonshadow_pointlights", this->m_scenegraph->_num_active_nonshadow_pointlights);

  // for (int i=0; i<this->m_scenegraph->_num_active_nonshadow_pointlights; i++)
  // {
  //   glActiveTexture(GL_TEXTURE11 + i);
  //   glBindTexture(GL_TEXTURE_CUBE_MAP, this->ren->depthCubeMaps[i]);
  //   sprintf(buffer, "nonshadow_pointlights[%d].depthCubemap", i);
  //   this->ren->active_shader->setInt(buffer, 11 + i);

  //   sprintf(buffer, "nonshadow_pointlights[%d].radius", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].radius);

  //   sprintf(buffer, "nonshadow_pointlights[%d].ambient", i);
  //   this->ren->active_shader->setVec3( buffer, this->m_scenegraph->pointlights[i].ambient);

  //   sprintf(buffer, "nonshadow_pointlights[%d].diffuse", i);
  //   this->ren->active_shader->setVec3( buffer, this->m_scenegraph->pointlights[i].diffuse);

  //   sprintf(buffer, "nonshadow_pointlights[%d].position", i);
  //   this->ren->active_shader->setVec3( buffer, this->m_scenegraph->pointlights[i].m_transform->getPos_worldspace());

  //   sprintf(buffer, "nonshadow_pointlights[%d].constant", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].constant);
    
  //   sprintf(buffer, "nonshadow_pointlights[%d].linear", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].linear);

  //   sprintf(buffer, "nonshadow_pointlights[%d].quadratic", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].quadratic);

  //   sprintf(buffer, "nonshadow_pointlights[%d].bias", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].bias);

  //   sprintf(buffer, "nonshadow_pointlights[%d].fog_constant", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_constant);

  //   sprintf(buffer, "nonshadow_pointlights[%d].fog_linear", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_linear);

  //   sprintf(buffer, "nonshadow_pointlights[%d].fog_quadratic", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_quadratic);

  //   sprintf(buffer, "nonshadow_pointlights[%d].fog_intensity", i);
  //   this->ren->active_shader->setFloat(buffer, this->m_scenegraph->pointlights[i].fog_intensity);
  // }





  // this->ren->active_shader->setInt("num_active_pointlights", this->m_scenegraph->_num_active_pointlights);
  // for (int i=0; i<this->m_scenegraph->_num_active_pointlights; i++)
  // {
  //   sprintf(buffer, "pointlights[%d].position", i);
  //   this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_pointlights[i]->m_transform->getPos_worldspace());

  //   sprintf(buffer, "pointlights[%d].ambient", i);
  //   this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_pointlights[i]->ambient);

  //   sprintf(buffer, "pointlights[%d].diffuse", i);
  //   this->ren->active_shader->setVec3(buffer,  this->m_scenegraph->sorted_pointlights[i]->diffuse);

  //   sprintf(buffer, "spotlights[%d].constant", i);
  //   this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_pointlights[i]->constant);

  //   sprintf(buffer, "pointlights[%d].linear", i);
  //   this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_pointlights[i]->linear);

  //   sprintf(buffer, "pointlights[%d].quadratic", i);
  //   this->ren->active_shader->setFloat(buffer,  this->m_scenegraph->sorted_pointlights[i]->quadratic);
  // }


  this->ren->active_shader->setInt("num_active_spotlights", this->m_scenegraph->_num_active_spotlights);
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


void Scene::drawDirLightDepthmap(void)
{
  glViewport(0, 0, this->ren->DIR_SHADOW_WIDTH, this->ren->DIR_SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, this->ren->dirlight_depthmapFBO);
  glBindTexture(GL_TEXTURE_2D, this->ren->dirlight_depthmap);
  glClear(GL_DEPTH_BUFFER_BIT);
      this->ren->useShader(SHADER_DIRSHADOW);
      this->ren->setupDirLightDepthmap(this->m_scenegraph->dirlight.position, this->m_scenegraph->dirlight.direction);
      this->drawGeometry();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Scene::drawPointLightDepthmaps(void)
{
  glViewport(0, 0, this->ren->POINT_SHADOW_WIDTH, this->ren->POINT_SHADOW_HEIGHT);
  this->ren->useShader(SHADER_POINTSHADOW);

  float aspect = (float)this->ren->POINT_SHADOW_WIDTH / (float)this->ren->POINT_SHADOW_HEIGHT;
  float near = 0.1f;

  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    if (this->m_scenegraph->pointlights[i].shadowmapped == false)
      continue;

    glBindFramebuffer(GL_FRAMEBUFFER, this->m_scenegraph->pointlights[i].FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    std::vector<glm::mat4> shadowTransforms;
    PointLight *pointlight = &World::scene.m_scenegraph->pointlights[i];
    glm::vec3 lightPos = pointlight->m_transform->getPos_worldspace();

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, pointlight->radius);

    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));


    char buffer[64];
    for (int i=0; i<6; i++)
    {
      sprintf(buffer, "shadowMatrices[%d]", i);
      this->ren->active_shader->setMat4(buffer, shadowTransforms[i]);
    }

    this->ren->active_shader->setVec3("lightPos", lightPos);
    this->ren->active_shader->setFloat("far_plane", pointlight->radius);

    this->drawGeometry();
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::drawDepthmaps(void)
{
  glDisable(GL_CULL_FACE);

  this->drawDirLightDepthmap();
  this->drawPointLightDepthmaps();

  glEnable(GL_CULL_FACE);
}


void Scene::physicsTick()
{
  for (auto &obj: this->m_scenegraph->m_object_instances)
  {
    if (obj.isHidden())
      continue;

    for (auto &obj2: this->m_scenegraph->m_object_instances)
      obj.collideWithObject(&obj2);

    obj.perFrameUpdate(this->ren);
  }
}


void Scene::drawGeometry()
{
  this->_lightsource_queue.clear();

  for (auto &obj: this->m_scenegraph->m_object_instances)
  {
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


void Scene::drawVolumetricLights(void)
{
  this->ren->sendVolumetricData();
  
  this->ren->active_shader->setMat4("projection", this->ren->cam.projection * this->ren->cam.view);
  this->ren->active_shader->setMat4("view", glm::mat4(1.0f));

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
  model = glm::scale(model, glm::vec3((float)this->ren->viewport_width, (float)this->ren->viewport_height, 1.0f));
  this->ren->active_shader->setMat4("model", glm::inverse(this->ren->cam.view) * model);


  glBindVertexArray(ren->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);


  for (auto &obj: this->m_scenegraph->m_object_instances)
  {
    if (obj.isHidden())
      continue;

    if (obj.hasGeometry())
    {
      obj.m_model->setTransform(obj.getTransform());
      this->ren->drawModel(obj.m_model);
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

