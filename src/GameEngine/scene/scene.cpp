#include "scene.h"



Player Scene::player = Player(&Render::ren);
ECS::EntityComponentSystem Scene::_entityComponentSystem;
SceneGraph Scene::scenegraph;


void Scene::init()
{
  Scene::player.init();
}


void Scene::clearColor(glm::vec3 color)
{
  Render::ren.clearColor = glm::vec4(color.x, color.y, color.z, 1.0f);
}


void Scene::sendLightsToShader()
{
  Render::ren.active_shader->setVec3( "clearColor", Render::ren.clearColor);
  Render::ren.active_shader->setFloat("fog_start", Render::ren.fog_start);
  Render::ren.active_shader->setFloat("fog_end", Render::ren.fog_end);
  Render::ren.active_shader->setFloat("far_plane",         25.0f );


  // Shadow mapped dirlight
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.ambient", Scene::scenegraph.dirlight.ambient);
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.diffuse", Scene::scenegraph.dirlight.diffuse);
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.position", Scene::scenegraph.dirlight.position);
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.direction", Scene::scenegraph.dirlight.direction);
  Render::ren.active_shader->setFloat( "shadowmapped_dirlight.bias", Scene::scenegraph.dirlight.bias);
  Render::ren.active_shader->setFloat( "shadowmapped_dirlight.fog_intensity", Scene::scenegraph.dirlight.fog_intensity);

  char buffer[64];


  Render::ren.active_shader->setInt("num_active_pointlights", Scene::scenegraph.num_active_pointlights);
  Render::ren.active_shader->setInt("num_shadow_pointlights", Scene::scenegraph.num_shadow_pointlights);

  // Non shadow-mapped pointlights
  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    sprintf(buffer, "pointlights[%d].radius", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->radius);

    sprintf(buffer, "pointlights[%d].ambient", i);
    Render::ren.active_shader->setVec3( buffer, Scene::scenegraph.active_pointlights[i]->ambient);

    sprintf(buffer, "pointlights[%d].diffuse", i);
    Render::ren.active_shader->setVec3( buffer, Scene::scenegraph.active_pointlights[i]->diffuse);

    sprintf(buffer, "pointlights[%d].position", i);
    Render::ren.active_shader->setVec3( buffer, Scene::scenegraph.active_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "pointlights[%d].constant", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->constant);
    
    sprintf(buffer, "pointlights[%d].linear", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->linear);

    sprintf(buffer, "pointlights[%d].quadratic", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->quadratic);

    sprintf(buffer, "pointlights[%d].bias", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->bias);

    sprintf(buffer, "pointlights[%d].fog_constant", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->fog_constant);

    sprintf(buffer, "pointlights[%d].fog_linear", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->fog_linear);

    sprintf(buffer, "pointlights[%d].fog_quadratic", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->fog_quadratic);

    sprintf(buffer, "pointlights[%d].fog_intensity", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.active_pointlights[i]->fog_intensity);
  }


  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    glActiveTexture(GL_TEXTURE11 + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::scenegraph.shadowmapped_pointlights[i]->depthCubemap);
    sprintf(buffer, "shadow_pointlights[%d].depthCubemap", i);
    Render::ren.active_shader->setInt(buffer, 11 + i);

    sprintf(buffer, "shadow_pointlights[%d].radius", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->radius);

    sprintf(buffer, "shadow_pointlights[%d].ambient", i);
    Render::ren.active_shader->setVec3( buffer, Scene::scenegraph.shadowmapped_pointlights[i]->ambient);

    sprintf(buffer, "shadow_pointlights[%d].diffuse", i);
    Render::ren.active_shader->setVec3( buffer, Scene::scenegraph.shadowmapped_pointlights[i]->diffuse);

    sprintf(buffer, "shadow_pointlights[%d].position", i);
    Render::ren.active_shader->setVec3( buffer, Scene::scenegraph.shadowmapped_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "shadow_pointlights[%d].constant", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->constant);
    
    sprintf(buffer, "shadow_pointlights[%d].linear", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->linear);

    sprintf(buffer, "shadow_pointlights[%d].quadratic", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->quadratic);

    sprintf(buffer, "shadow_pointlights[%d].bias", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->bias);

    sprintf(buffer, "shadow_pointlights[%d].fog_constant", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->fog_constant);

    sprintf(buffer, "shadow_pointlights[%d].fog_linear", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->fog_linear);

    sprintf(buffer, "shadow_pointlights[%d].fog_quadratic", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->fog_quadratic);

    sprintf(buffer, "shadow_pointlights[%d].fog_intensity", i);
    Render::ren.active_shader->setFloat(buffer, Scene::scenegraph.shadowmapped_pointlights[i]->fog_intensity);
  }

  // Render::ren.active_shader->setInt("num_active_spotlights", SceneNamespace::scenegraph._num_active_spotlights);
  // for (int i=0; i<SceneNamespace::scenegraph._num_active_spotlights; i++)
  // {
  //   sprintf(buffer, "spotlights[%d].position", i);
  //   Render::ren.active_shader->setVec3(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->m_transform->getPos_worldspace());

  //   sprintf(buffer, "spotlights[%d].direction", i);
  //   Render::ren.active_shader->setVec3(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->m_transform->getRot());

  //   sprintf(buffer, "spotlights[%d].ambient", i);
  //   Render::ren.active_shader->setVec3(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->ambient);

  //   sprintf(buffer, "spotlights[%d].diffuse", i);
  //   Render::ren.active_shader->setVec3(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->diffuse);

  //   sprintf(buffer, "spotlights[%d].constant", i);
  //   Render::ren.active_shader->setFloat(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->constant);

  //   sprintf(buffer, "spotlights[%d].linear", i);
  //   Render::ren.active_shader->setFloat(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->linear);

  //   sprintf(buffer, "spotlights[%d].quadratic", i);
  //   Render::ren.active_shader->setFloat(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->quadratic);

  //   sprintf(buffer, "spotlights[%d].inner_cutoff", i);
  //   Render::ren.active_shader->setFloat(buffer,  glm::cos(glm::radians(SceneNamespace::scenegraph.sorted_spotlights[i]->inner_cutoff)));

  //   sprintf(buffer, "spotlights[%d].outer_cutoff", i);
  //   Render::ren.active_shader->setFloat(buffer,  glm::cos(glm::radians(SceneNamespace::scenegraph.sorted_spotlights[i]->outer_cutoff)));

  //   sprintf(buffer, "spotlights[%d].intensity", i);
  //   Render::ren.active_shader->setFloat(buffer,  SceneNamespace::scenegraph.sorted_spotlights[i]->intensity);
  // }

  Render::ren.active_shader->setVec3("viewPos", Render::ren.cam.m_transform->getPos_worldspace());
}


void Scene::sendVolumetricData()
{
  Render::ren.active_shader->setInt("volumetrics.num_samples",         Render::ren.volumetrics.num_samples);
  Render::ren.active_shader->setFloat("volumetrics.step_size",         Render::ren.volumetrics.step_size);
  Render::ren.active_shader->setFloat("volumetrics.step_multiplier",   Render::ren.volumetrics.step_multiplier);
  Render::ren.active_shader->setInt("volumetrics.resolution_divisor",  Render::ren.volumetrics.resolution_divisor);
  Render::ren.active_shader->setInt("volumetrics.num_blur_passes",     Render::ren.volumetrics.num_blur_passes);
  

  SceneGraph *scenegraph = &Scene::scenegraph;
  char buffer[256];

  Render::ren.active_shader->setInt("num_volumetric_pointlights", scenegraph->num_volumetric_pointlights);
  Render::ren.active_shader->setInt("num_shadow_pointlights", scenegraph->num_shadowmapped_volumetric_pointlights);

  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    sprintf(buffer, "volumetric_pointlights[%d].radius", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->radius);

    sprintf(buffer, "volumetric_pointlights[%d].ambient", i);
    Render::ren.active_shader->setVec3( buffer, scenegraph->volumetric_pointlights[i]->ambient);

    sprintf(buffer, "volumetric_pointlights[%d].diffuse", i);
    Render::ren.active_shader->setVec3( buffer, scenegraph->volumetric_pointlights[i]->diffuse);

    sprintf(buffer, "volumetric_pointlights[%d].position", i);
    Render::ren.active_shader->setVec3( buffer, scenegraph->volumetric_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "volumetric_pointlights[%d].constant", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->constant);
    
    sprintf(buffer, "volumetric_pointlights[%d].linear", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->linear);

    sprintf(buffer, "volumetric_pointlights[%d].quadratic", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->quadratic);

    sprintf(buffer, "volumetric_pointlights[%d].bias", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->bias);

    sprintf(buffer, "volumetric_pointlights[%d].fog_constant", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->fog_constant);

    sprintf(buffer, "volumetric_pointlights[%d].fog_linear", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->fog_linear);

    sprintf(buffer, "volumetric_pointlights[%d].fog_quadratic", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->fog_quadratic);

    sprintf(buffer, "volumetric_pointlights[%d].fog_intensity", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->volumetric_pointlights[i]->fog_intensity);
  }

  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    glActiveTexture(GL_TEXTURE11 + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scenegraph->shadowmapped_volumetric_pointlights[i]->depthCubemap);
    sprintf(buffer, "shadow_pointlights[%d].depthCubemap", i);
    Render::ren.active_shader->setInt(buffer, 11 + i);


    sprintf(buffer, "shadow_pointlights[%d].radius", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->radius);

    sprintf(buffer, "shadow_pointlights[%d].ambient", i);
    Render::ren.active_shader->setVec3( buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->ambient);

    sprintf(buffer, "shadow_pointlights[%d].diffuse", i);
    Render::ren.active_shader->setVec3( buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->diffuse);

    sprintf(buffer, "shadow_pointlights[%d].position", i);
    Render::ren.active_shader->setVec3( buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "shadow_pointlights[%d].constant", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->constant);
    
    sprintf(buffer, "shadow_pointlights[%d].linear", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->linear);

    sprintf(buffer, "shadow_pointlights[%d].quadratic", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->quadratic);

    sprintf(buffer, "shadow_pointlights[%d].bias", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->bias);

    sprintf(buffer, "shadow_pointlights[%d].fog_constant", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->fog_constant);

    sprintf(buffer, "shadow_pointlights[%d].fog_linear", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->fog_linear);

    sprintf(buffer, "shadow_pointlights[%d].fog_quadratic", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->fog_quadratic);

    sprintf(buffer, "shadow_pointlights[%d].fog_intensity", i);
    Render::ren.active_shader->setFloat(buffer, scenegraph->shadowmapped_volumetric_pointlights[i]->fog_intensity);
  }
}



void Scene::defaultScene()
{
  Scene::scenegraph.clearScene();
  Scene::scenegraph.newObjectInstance("player");
  GameObject *obj = Scene::scenegraph.rearObjectPtr();
  Scene::player.useGameObject(obj);
}


void Scene::importScene(std::string filepath)
{
  Scene::scenegraph.importScene(filepath, &Scene::player);
}


void Scene::drawDirLightDepthmap()
{
  Render::ren.useShader(SHADER_DIRSHADOW);
  glViewport(0, 0, DIRLIGHT_RES, DIRLIGHT_RES);

  Render::ren.setupDirLightDepthmap(Scene::scenegraph.dirlight.position, Scene::scenegraph.dirlight.direction);
  for (size_t i=0; i<NUM_SHADOW_CASCADES; i++)
  {
    RenderUtil::bindWrite_cascade(&Render::ren.cascaded_rsm, i);
    glClear(GL_DEPTH_BUFFER_BIT);
    Render::ren.active_shader->setMat4("lightSpaceMatrix", Render::ren.cascaded_rsm.lightSpaceMatrices[i]);
    Scene::drawGeometry();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, Render::ren.viewport_width, Render::ren.viewport_height);
}


void Scene::drawPointLightDepthmaps()
{
  Render::ren.useShader(SHADER_POINTSHADOW);

  glViewport(0, 0, Render::ren.POINT_SHADOW_WIDTH, Render::ren.POINT_SHADOW_HEIGHT);

  float aspect = (float)Render::ren.POINT_SHADOW_WIDTH / (float)Render::ren.POINT_SHADOW_HEIGHT;
  float near = 0.1f;

  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    PointLight *pointlight = &Scene::scenegraph.pointlights[i];

    if (pointlight->shadowmapped == false)
      continue;

    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, pointlight->FBO) );
    GLCALL( glClear(GL_DEPTH_BUFFER_BIT) );

    std::vector<glm::mat4> shadowTransforms;
    glm::vec3 lightPos = pointlight->m_transform->getPos_worldspace();

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, pointlight->radius);


    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));


    for (int j=0; j<6; j++)
      Render::ren.active_shader->setMat4("shadowMatrices[" + std::to_string(j) + "]", shadowTransforms[j]);


    Render::ren.active_shader->setVec3("lightPos", lightPos);
    Render::ren.active_shader->setFloat("far_plane", pointlight->radius);

    Scene::drawGeometry();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Scene::drawDepthmaps()
{
  glCullFace(GL_FRONT);

  Scene::drawDirLightDepthmap();
  
  GLCALL( glDisable(GL_DEPTH_CLAMP) );
  Scene::drawPointLightDepthmaps();
  GLCALL( glEnable(GL_DEPTH_CLAMP) );

  glCullFace(GL_BACK);
}


void Scene::animationTick()
{
  std::list<GameObject *> *objects = Scene::scenegraph.getObjects(GAMEOBJECT_ACTOR, GameObjectFlag::ANIMATED);
  for (GameObject *object: *objects)
  {
    object->getAnimation()->getArmature()->computePose(object->getAnimation()->getTime());
    object->getAnimation()->advance(Render::ren.deltaTime);
  }
}


void Scene::physicsTick_actor_terrain()
{
  std::list<GameObject *> *actor_list = Scene::scenegraph.getObjects(GAMEOBJECT_ACTOR);
  std::list<GameObject *> *actorAnimated_list = Scene::scenegraph.getObjects(GAMEOBJECT_ACTOR, GameObjectFlag::ANIMATED);
  std::list<GameObject *> *terrain_list = Scene::scenegraph.getObjects(GAMEOBJECT_TERRAIN);
  std::list<GameObject *> *static_list = Scene::scenegraph.getObjects(GAMEOBJECT_STATIC);

  for (auto &terrain: *terrain_list)
  {
    for (auto &actor: *actor_list)
      actor->collideWithObject(terrain);

    for (auto &actor: *actorAnimated_list)
      actor->collideWithObject(terrain);
  }


  for (auto &staticobj: *static_list)
  {
    for (auto &actor: *actor_list)
      actor->collideWithObject(staticobj);

    for (auto &actor: *actorAnimated_list)
      actor->collideWithObject(staticobj);
  }
}


void Scene::physicsTick_actor_actor()
{

}


void Scene::physicsTick()
{
  std::list<GameObject *> *terrain_list = Scene::scenegraph.getObjects(GAMEOBJECT_TERRAIN);
  for (auto &terrain: *terrain_list)
    Scene::scenegraph.player_object->collideWithObject(terrain);

  std::list<GameObject *> *static_list = Scene::scenegraph.getObjects(GAMEOBJECT_STATIC);
  for (auto &staticobj: *static_list)
    Scene::scenegraph.player_object->collideWithObject(staticobj);

  for (auto &staticobj1: *static_list)
    for (auto &staticobj2: *static_list)
        staticobj1->collideWithObject(staticobj2);


  Scene::physicsTick_actor_terrain();
  Scene::physicsTick_actor_actor();
  
  Scene::animationTick();


  for (auto &obj: Scene::scenegraph.m_object_instances)
  {
    obj.perFrameUpdate(&Render::ren);
  }
}


void Scene::perFrameUpdate()
{
  Scene::physicsTick();

  // Frustum culling
  //------------------------------------------------------
  Frustum *frustum = Render::ren.getFrustum();
  Scene::scenegraph.cullObjects(frustum);
  Scene::scenegraph.sortLights(frustum);
  // Scene::scenegraph.perFrameUpdate();
  //------------------------------------------------------
}



void Scene::drawBackground()
{
  Render::ren.useShader(SHADER_BACKGROUND);
  Render::ren.active_shader->setMat4("projection", Render::ren.cam.projection);
  Render::ren.active_shader->setMat4("view", Render::ren.cam.view);
  
  float aspect = Render::ren.viewport_width / Render::ren.viewport_height;
  float height = Render::ren.far_plane * tan(Render::ren.fov);
  float width = aspect * height;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(width, height, 1.0f));
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.99 * Render::ren.far_plane));
  Render::ren.active_shader->setMat4("model", model);
  Render::ren.active_shader->setVec3("skyColor", Render::ren.clearColor);
  Render::ren.active_shader->setVec3("horizonColor", Render::ren.horizonColor);
  Render::ren.active_shader->setFloat("start", Render::ren.transition_start);
  Render::ren.active_shader->setFloat("stop", Render::ren.transition_stop);


  Render::ren.active_shader->setVec3("ViewPos", Render::ren.cam.m_transform->getPos_worldspace());
  Render::ren.active_shader->setVec3("SunPos", Scene::scenegraph.dirlight.position);

  GLCALL(glBindVertexArray(Render::ren.quadVAO));
  GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));
}


void Scene::drawTerrain()
{
  Render::ren.useShader(SHADER_TERRAIN);

  for (auto &obj: *Scene::scenegraph.getObjects(GAMEOBJECT_TERRAIN))
  {
    TerrainComponent *tc = obj->getComponents()->getTerrainComponent();
    Render::ren.drawTerrain(obj->getModel(), obj->getTransform(), tc->threshold, tc->epsilon);
  }
}


void Scene::drawStatic()
{
  Render::ren.useShader(SHADER_ACTOR);

  for (auto &obj: *Scene::scenegraph.getObjects(GAMEOBJECT_STATIC))
    Render::ren.drawModel(obj->getModel(), obj->getTransform());
}


void Scene::drawBillboards()
{
  Render::ren.useShader(SHADER_BILLBOARD_FIXED);
  
  for (auto &obj: *Scene::scenegraph.getVisibleInstancesByType(GAMEOBJECT_BILLBOARD))
    Render::ren.drawBillboard(obj->getModel(), obj->getTransform());
}


void Scene::drawBillboardsInstanced()
{
  Render::ren.useShader(SHADER_BILLBOARD_FOLLOW);

  glm::mat4 view_noTranslate = Render::ren.cam.view;
  view_noTranslate[3] = glm::vec4(0.0f, 0.0f, 0.f, 1.0f);
  Render::ren.active_shader->setMat4("view_noTranslate", view_noTranslate);
  Render::ren.active_shader->setInt("diffuseMap", 0);

  std::map<std::string, InstanceData> *map = Scene::scenegraph.getInstanceData();
  for (auto it = map->begin(); it != map->end(); ++it)
  {
    InstanceData *instance_data = &(it)->second;

    Render::ren.drawModelInstanced(instance_data->model, instance_data);
  }
}


void Scene::drawActors()
{
  Render::ren.useShader(SHADER_ACTOR);
  Render::ren.active_shader->setInt("material.diffuseMap", 0);
  Render::ren.active_shader->setInt("material.specularMap", 1);
  Render::ren.active_shader->setInt("material.normalMap", 2);
  Render::ren.active_shader->setInt("material.emissionMap", 3);

  for (auto &obj: *Scene::scenegraph.getObjects(GAMEOBJECT_ACTOR))
    Render::ren.drawModel(obj->getModel(), obj->getTransform());

  Render::ren.active_shader->setVec3("emission", glm::vec3(0.0f));
  Render::ren.active_shader->setFloat("emission_scale", 0.0f);


  Render::ren.useShader(SHADER_ACTOR_ANIMATED);
  Render::ren.active_shader->setInt("material.diffuseMap", 0);
  Render::ren.active_shader->setInt("material.specularMap", 1);
  Render::ren.active_shader->setInt("material.normalMap", 2);
  Render::ren.active_shader->setInt("material.emissionMap", 3);

  for (auto &obj: *Scene::scenegraph.getObjects(GAMEOBJECT_ACTOR, GameObjectFlag::ANIMATED))
    Render::ren.drawModelAnimated(obj->getModel(), obj->getTransform(), obj->getAnimationController());

  Render::ren.active_shader->setVec3("emission", glm::vec3(0.0f));
  Render::ren.active_shader->setFloat("emission_scale", 0.0f);
}


void Scene::drawLightsources()
{
  Render::ren.useShader(SHADER_LIGHTSOURCE);

  for (GameObject *obj: *Scene::scenegraph.getObjects(GAMEOBJECT_LIGHTSOURCE))
    Render::ren.drawLightSource(obj->getModel(), obj->getTransform(), obj->getComponents()->getPointLightComponent()->diffuse);
}


void Scene::drawGeometry_batched()
{
  Scene::drawTerrain();
  Scene::drawStatic();
  Scene::drawActors();
  Scene::drawLightsources();

  
  GLCALL( glDisable(GL_CULL_FACE) );
  Scene::drawBillboards();
  Scene::drawBillboardsInstanced();
  GLCALL( glEnable(GL_CULL_FACE) );

    Render::ren.useShader(SHADER_WIREFRAME);

  if (Render::ren.getDebugData()->getDebugFlag(RenderDebugFlag::DrawBoundingSpheres))
  {
    Render::ren.active_shader->setMat4("projection", Render::ren.cam.projection);
    Render::ren.active_shader->setMat4("view", Render::ren.cam.view);

    for (GameObject *obj: *Scene::scenegraph.getVisibleInstancesByType(GAMEOBJECT_TERRAIN))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->getCullingData()->bounding_sphere_pos, obj->getCullingData()->bounding_sphere_radius, obj->getTransform());

     for (GameObject *obj: *Scene::scenegraph.getVisibleInstancesByType(GAMEOBJECT_STATIC))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->getCullingData()->bounding_sphere_pos, obj->getCullingData()->bounding_sphere_radius, obj->getTransform());   

    for (GameObject *obj: *Scene::scenegraph.getVisibleInstancesByType(GAMEOBJECT_ACTOR))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->getCullingData()->bounding_sphere_pos, obj->getCullingData()->bounding_sphere_radius, obj->getTransform());
  
    for (GameObject *obj: *Scene::scenegraph.getVisibleInstancesByType(GAMEOBJECT_BILLBOARD))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->getCullingData()->bounding_sphere_pos, obj->getCullingData()->bounding_sphere_radius, obj->getTransform());
  
    for (GameObject *obj: *Scene::scenegraph.getObjects(GAMEOBJECT_EMPTY))
      Render::ren.drawPrimitive_box(obj->getTransform());
  }
  
  if (Render::ren.getDebugData()->getDebugFlag(RenderDebugFlag::DrawColliders))
  {
    Render::ren.active_shader->setMat4("projection", Render::ren.cam.projection);
    Render::ren.active_shader->setMat4("view", Render::ren.cam.view);

    for (GameObject *obj: Scene::scenegraph.getObjects(GameObjectFlag::GEOMETRY))
    {
      if (obj->getComponents()->hasComponent(COMPONENT_SPHERE_COLLIDER))
      {
        glm::vec3 pos = obj->getTransform()->getPos_worldspace();
        pos += glm::vec3(obj->getTransform()->getModelMatrix() * glm::vec4(obj->spherecollider.position_offset, 1.0f));
        float radius = obj->spherecollider.radius;
        Render::ren.drawPrimitive(PRIMITIVE_SPHERE, pos, radius, obj->getTransform());
      }
    }
  }
}


void Scene::drawGeometry()
{
  for (GameObject *object: Scene::scenegraph.getObjects(GameObjectFlag::GEOMETRY))
  {
    Render::ren.drawModel(object->getModelLOD()->getShadowLOD_model(), object->getTransform());
  }
}


void Scene::drawGeometry_animated()
{
  for (auto &object: *Scene::scenegraph.getObjects(GAMEOBJECT_ACTOR, GameObjectFlag::ANIMATED))
    Render::ren.drawModelAnimated(object->getModelLOD()->getShadowLOD_model(), object->getTransform(), object->getAnimationController());
}

