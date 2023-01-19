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


  this->ren->active_shader->setInt("num_active_pointlights", this->m_scenegraph->num_active_pointlights);
  this->ren->active_shader->setInt("num_shadow_pointlights", this->m_scenegraph->num_shadow_pointlights);

  // Non shadow-mapped pointlights
  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    sprintf(buffer, "pointlights[%d].radius", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->radius);

    sprintf(buffer, "pointlights[%d].ambient", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->sorted_active_pointlights[i]->ambient);

    sprintf(buffer, "pointlights[%d].diffuse", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->sorted_active_pointlights[i]->diffuse);

    sprintf(buffer, "pointlights[%d].position", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->sorted_active_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "pointlights[%d].constant", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->constant);
    
    sprintf(buffer, "pointlights[%d].linear", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->linear);

    sprintf(buffer, "pointlights[%d].quadratic", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->quadratic);

    sprintf(buffer, "pointlights[%d].bias", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->bias);

    sprintf(buffer, "pointlights[%d].fog_constant", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->fog_constant);

    sprintf(buffer, "pointlights[%d].fog_linear", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->fog_linear);

    sprintf(buffer, "pointlights[%d].fog_quadratic", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->fog_quadratic);

    sprintf(buffer, "pointlights[%d].fog_intensity", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_active_pointlights[i]->fog_intensity);
  }


  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    glActiveTexture(GL_TEXTURE11 + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_scenegraph->sorted_shadow_pointlights[i]->depthCubemap);
    sprintf(buffer, "shadow_pointlights[%d].depthCubemap", i);
    this->ren->active_shader->setInt(buffer, 11 + i);

    sprintf(buffer, "shadow_pointlights[%d].radius", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->radius);

    sprintf(buffer, "shadow_pointlights[%d].ambient", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->ambient);

    sprintf(buffer, "shadow_pointlights[%d].diffuse", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->diffuse);

    sprintf(buffer, "shadow_pointlights[%d].position", i);
    this->ren->active_shader->setVec3( buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "shadow_pointlights[%d].constant", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->constant);
    
    sprintf(buffer, "shadow_pointlights[%d].linear", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->linear);

    sprintf(buffer, "shadow_pointlights[%d].quadratic", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->quadratic);

    sprintf(buffer, "shadow_pointlights[%d].bias", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->bias);

    sprintf(buffer, "shadow_pointlights[%d].fog_constant", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->fog_constant);

    sprintf(buffer, "shadow_pointlights[%d].fog_linear", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->fog_linear);

    sprintf(buffer, "shadow_pointlights[%d].fog_quadratic", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->fog_quadratic);

    sprintf(buffer, "shadow_pointlights[%d].fog_intensity", i);
    this->ren->active_shader->setFloat(buffer, this->m_scenegraph->sorted_shadow_pointlights[i]->fog_intensity);
  }

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
  GLCALL(glViewport(0, 0, this->ren->DIR_SHADOW_WIDTH, this->ren->DIR_SHADOW_HEIGHT));
  GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, this->ren->dirlight_depthmapFBO));
  GLCALL(glBindTexture(GL_TEXTURE_2D, this->ren->dirlight_depthmap));
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


void Scene::physicsTick_actor_terrain(void)
{
  std::list<GameObject *> *actor_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_ACTOR);
  std::list<GameObject *> *terrain_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_TERRAIN);

  for (auto &actor: *actor_list)
    for (auto &terrain: *terrain_list)
      actor->collideWithObject(terrain);
}


void Scene::physicsTick_actor_actor(void)
{

}


void Scene::physicsTick(void)
{
  std::list<GameObject *> *terrain_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_TERRAIN);
  for (auto &terrain: *terrain_list)
    this->m_scenegraph->player_object->collideWithObject(terrain);

  std::list<GameObject *> *static_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_STATIC);
  for (auto &staticobj: *static_list)
    this->m_scenegraph->player_object->collideWithObject(staticobj);

  this->physicsTick_actor_terrain();
  this->physicsTick_actor_actor();

  for (auto &obj: this->m_scenegraph->m_object_instances)
  {
    obj.perFrameUpdate(this->ren);
  }
}


void Scene::drawBackground()
{
  this->ren->useShader(SHADER_BACKGROUND);
  this->ren->active_shader->setMat4("projection", this->ren->cam.projection);
  this->ren->active_shader->setMat4("view", this->ren->cam.view);
  
  float aspect = ren->viewport_width / ren->viewport_height;
  float height = ren->far_plane * tan(ren->fov);
  float width = aspect * height;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(width, height, 1.0f));
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.99 * this->ren->far_plane));
  this->ren->active_shader->setMat4("model", model);
  this->ren->active_shader->setVec3("clearColor", this->ren->clearColor);

  this->ren->active_shader->setVec3("ViewPos", this->ren->cam.m_transform->getPos_worldspace());
  this->ren->active_shader->setVec3("SunPos", this->m_scenegraph->dirlight.position);

  GLCALL(glBindVertexArray(this->ren->quadVAO));
  GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));
}


void Scene::drawGeometry()
{
  std::list<GameObject *> *terrain_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_TERRAIN);
  for (auto &terrain: *terrain_list)
    this->ren->drawModel(terrain->m_model, terrain->getTransform());
   
  std::list<GameObject *> *static_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_STATIC);
  for (auto &staticobj: *static_list)
    this->ren->drawModel(staticobj->m_model, staticobj->getTransform());

  std::list<GameObject *> *actor_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_ACTOR);
  for (auto &actor: *actor_list)
    this->ren->drawModel(actor->m_model, actor->getTransform());

  std::list<GameObject *> *billboard_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_BILLBOARD, INSTANCING_OFF);
  for (auto &actor: *billboard_list)
    this->ren->drawModel(actor->m_model, actor->getTransform());
}


void Scene::drawGeometry_batched()
{
  this->drawTerrain();
  this->drawStatic();
  this->drawActors();
  this->drawLightsources();


  
  GLCALL( glDisable(GL_CULL_FACE) );
  this->drawBillboards();
  this->drawBillboardsInstanced();
  GLCALL( glEnable(GL_CULL_FACE) );
}


void Scene::drawTerrain()
{
  ren->useShader(SHADER_TERRAIN);

  std::list<GameObject *> *terrain_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_TERRAIN);
  for (auto &obj: *terrain_list)
  {
    TerrainComponent tc = obj->terrain_components[0].terrain_component;
    this->ren->drawTerrain(obj->m_model, obj->getTransform(), tc.threshold, tc.epsilon);
  }
}


void Scene::drawStatic()
{
  ren->useShader(SHADER_ACTOR);

  std::list<GameObject *> *static_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_STATIC);
  for (auto &obj: *static_list)
  {
    this->ren->drawModel(obj->m_model, obj->getTransform());
  }
}


void Scene::drawBillboards()
{
  ren->useShader(SHADER_BILLBOARD_FIXED);
  std::list<GameObject *> *billboard_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_BILLBOARD, INSTANCING_OFF);
  for (auto &obj: *billboard_list)
  {
    this->ren->drawBillboard(obj->m_model, obj->getTransform());
  }
}


void Scene::drawBillboardsInstanced()
{
  ren->useShader(SHADER_BILLBOARD_FOLLOW);


  glm::mat4 view_noTranslate = this->ren->cam.view;
  view_noTranslate[3] = glm::vec4(0.0f, 0.0f, 0.f, 1.0f);
  this->ren->active_shader->setMat4("view_noTranslate", view_noTranslate);
  this->ren->active_shader->setInt("diffuseMap", 0);


  std::map<std::string, InstanceData> *map = this->m_scenegraph->getInstanceData();
  for (auto it = map->begin(); it != map->end(); ++it)
  {
    InstanceData *instance_data = &(it)->second;

    this->ren->drawModelInstanced(instance_data->model, instance_data);
  }
}


void Scene::drawActors()
{
  ren->useShader(SHADER_ACTOR);

  this->ren->active_shader->setInt("material.diffuseMap", 0);
  this->ren->active_shader->setInt("material.specularMap", 1);
  this->ren->active_shader->setInt("material.normalMap", 2);
  this->ren->active_shader->setInt("material.emissionMap", 3);

  std::list<GameObject *> *actor_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_ACTOR);
  for (auto &obj: *actor_list)
  {
    this->ren->active_shader->setVec3("emission", obj->emission);
    this->ren->active_shader->setFloat("emission_scale", obj->emission_scale);

    this->ren->drawModel(obj->m_model, obj->getTransform());
  }
  this->ren->active_shader->setVec3("emission", glm::vec3(0.0f));
  this->ren->active_shader->setFloat("emission_scale", 0.0f);
}


void Scene::drawLightsources()
{
  ren->useShader(SHADER_LIGHTSOURCE);

  std::list<GameObject *> *lightsource_list = this->m_scenegraph->getInstancesByType(GAMEOBJECT_LIGHTSOURCE);
  for (GameObject *obj: *lightsource_list)
  {
    this->ren->drawLightSource(obj->m_model, obj->getTransform(), *obj->lightsource_components[0].diffuse);
  }
}


Scene World::scene;

