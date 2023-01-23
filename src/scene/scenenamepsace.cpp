#include "scene.h"



Player SceneNamespace::player = Player(&Render::ren);
SceneGraph SceneNamespace::scenegraph;



void SceneNamespace::sendLightsToShader()
{
  Render::ren.active_shader->setVec3( "clearColor", Render::ren.clearColor);
  Render::ren.active_shader->setFloat("fog_start", Render::ren.fog_start);
  Render::ren.active_shader->setFloat("fog_end", Render::ren.fog_end);
  Render::ren.active_shader->setFloat("far_plane",         25.0f );


  // Shadow mapped dirlight
  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_2D, Render::ren.dirlight_depthmap);
  Render::ren.active_shader->setInt(   "depthmap_dirlight", 10    );
  Render::ren.active_shader->setMat4(  "dir_lightSpaceMatrix", Render::ren.lightSpaceMatrix);

  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.ambient", SceneNamespace::scenegraph.dirlight.ambient);
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.diffuse", SceneNamespace::scenegraph.dirlight.diffuse);
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.position", SceneNamespace::scenegraph.dirlight.position);
  Render::ren.active_shader->setVec3(  "shadowmapped_dirlight.direction", SceneNamespace::scenegraph.dirlight.direction);
  Render::ren.active_shader->setFloat( "shadowmapped_dirlight.bias", SceneNamespace::scenegraph.dirlight.bias);
  Render::ren.active_shader->setFloat( "shadowmapped_dirlight.fog_intensity", SceneNamespace::scenegraph.dirlight.fog_intensity);

  char buffer[64];


  Render::ren.active_shader->setInt("num_active_pointlights", SceneNamespace::scenegraph.num_active_pointlights);
  Render::ren.active_shader->setInt("num_shadow_pointlights", SceneNamespace::scenegraph.num_shadow_pointlights);

  // Non shadow-mapped pointlights
  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    sprintf(buffer, "pointlights[%d].radius", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->radius);

    sprintf(buffer, "pointlights[%d].ambient", i);
    Render::ren.active_shader->setVec3( buffer, SceneNamespace::scenegraph.active_pointlights[i]->ambient);

    sprintf(buffer, "pointlights[%d].diffuse", i);
    Render::ren.active_shader->setVec3( buffer, SceneNamespace::scenegraph.active_pointlights[i]->diffuse);

    sprintf(buffer, "pointlights[%d].position", i);
    Render::ren.active_shader->setVec3( buffer, SceneNamespace::scenegraph.active_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "pointlights[%d].constant", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->constant);
    
    sprintf(buffer, "pointlights[%d].linear", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->linear);

    sprintf(buffer, "pointlights[%d].quadratic", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->quadratic);

    sprintf(buffer, "pointlights[%d].bias", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->bias);

    sprintf(buffer, "pointlights[%d].fog_constant", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->fog_constant);

    sprintf(buffer, "pointlights[%d].fog_linear", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->fog_linear);

    sprintf(buffer, "pointlights[%d].fog_quadratic", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->fog_quadratic);

    sprintf(buffer, "pointlights[%d].fog_intensity", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.active_pointlights[i]->fog_intensity);
  }


  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    glActiveTexture(GL_TEXTURE11 + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->depthCubemap);
    sprintf(buffer, "shadow_pointlights[%d].depthCubemap", i);
    Render::ren.active_shader->setInt(buffer, 11 + i);

    sprintf(buffer, "shadow_pointlights[%d].radius", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->radius);

    sprintf(buffer, "shadow_pointlights[%d].ambient", i);
    Render::ren.active_shader->setVec3( buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->ambient);

    sprintf(buffer, "shadow_pointlights[%d].diffuse", i);
    Render::ren.active_shader->setVec3( buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->diffuse);

    sprintf(buffer, "shadow_pointlights[%d].position", i);
    Render::ren.active_shader->setVec3( buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->m_transform->getPos_worldspace());

    sprintf(buffer, "shadow_pointlights[%d].constant", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->constant);
    
    sprintf(buffer, "shadow_pointlights[%d].linear", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->linear);

    sprintf(buffer, "shadow_pointlights[%d].quadratic", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->quadratic);

    sprintf(buffer, "shadow_pointlights[%d].bias", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->bias);

    sprintf(buffer, "shadow_pointlights[%d].fog_constant", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->fog_constant);

    sprintf(buffer, "shadow_pointlights[%d].fog_linear", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->fog_linear);

    sprintf(buffer, "shadow_pointlights[%d].fog_quadratic", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->fog_quadratic);

    sprintf(buffer, "shadow_pointlights[%d].fog_intensity", i);
    Render::ren.active_shader->setFloat(buffer, SceneNamespace::scenegraph.shadowmapped_pointlights[i]->fog_intensity);
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


void SceneNamespace::defaultScene()
{
  SceneNamespace::scenegraph.clearScene();
  SceneNamespace::scenegraph.newObjectInstance("player");
  GameObject *obj = SceneNamespace::scenegraph.rearObjectPtr();
  SceneNamespace::player.useGameObject(obj);
}


void SceneNamespace::importScene(std::string filepath)
{
  SceneNamespace::scenegraph.importScene(filepath, &SceneNamespace::player);

}


void SceneNamespace::drawDirLightDepthmap()
{
  GLCALL(glViewport(0, 0, Render::ren.DIR_SHADOW_WIDTH, Render::ren.DIR_SHADOW_HEIGHT));
  GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, Render::ren.dirlight_depthmapFBO));
  GLCALL(glBindTexture(GL_TEXTURE_2D, Render::ren.dirlight_depthmap));
  glClear(GL_DEPTH_BUFFER_BIT);
    Render::ren.useShader(SHADER_DIRSHADOW);
    Render::ren.setupDirLightDepthmap(SceneNamespace::scenegraph.dirlight.position, SceneNamespace::scenegraph.dirlight.direction);
    SceneNamespace::drawGeometry();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void SceneNamespace::drawPointLightDepthmaps()
{
  glViewport(0, 0, Render::ren.POINT_SHADOW_WIDTH, Render::ren.POINT_SHADOW_HEIGHT);
  Render::ren.useShader(SHADER_POINTSHADOW);

  float aspect = (float)Render::ren.POINT_SHADOW_WIDTH / (float)Render::ren.POINT_SHADOW_HEIGHT;
  float near = 0.1f;

  for (int i=0; i<MAX_POINTLIGHTS; i++)
  {
    if (SceneNamespace::scenegraph.pointlights[i].shadowmapped == false)
      continue;

    glBindFramebuffer(GL_FRAMEBUFFER, SceneNamespace::scenegraph.pointlights[i].FBO);
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
      Render::ren.active_shader->setMat4(buffer, shadowTransforms[i]);
    }

    Render::ren.active_shader->setVec3("lightPos", lightPos);
    Render::ren.active_shader->setFloat("far_plane", pointlight->radius);

    SceneNamespace::drawGeometry();
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void SceneNamespace::drawDepthmaps()
{
  glDisable(GL_CULL_FACE);

  SceneNamespace::drawDirLightDepthmap();
  SceneNamespace::drawPointLightDepthmaps();

  glEnable(GL_CULL_FACE);
}




void SceneNamespace::physicsTick_actor_terrain()
{
  std::list<GameObject *> *actor_list = SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_ACTOR);
  std::list<GameObject *> *terrain_list = SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_TERRAIN);

  for (auto &actor: *actor_list)
    for (auto &terrain: *terrain_list)
      actor->collideWithObject(terrain);
}


void SceneNamespace::physicsTick_actor_actor()
{

}



void SceneNamespace::physicsTick()
{
  std::list<GameObject *> *terrain_list = SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_TERRAIN);
  for (auto &terrain: *terrain_list)
    SceneNamespace::scenegraph.player_object->collideWithObject(terrain);

  std::list<GameObject *> *static_list = SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_STATIC);
  for (auto &staticobj: *static_list)
    SceneNamespace::scenegraph.player_object->collideWithObject(staticobj);


  SceneNamespace::physicsTick_actor_terrain();
  SceneNamespace::physicsTick_actor_actor();
  

  for (auto &obj: SceneNamespace::scenegraph.m_object_instances)
  {
    obj.perFrameUpdate(&Render::ren);
  }
}


void SceneNamespace::perFrameUpdate()
{
  SceneNamespace::physicsTick();

  // Frustum culling
  //------------------------------------------------------
  Frustum *frustum = Render::ren.getFrustum();
  SceneNamespace::scenegraph.cullObjects(frustum);
  SceneNamespace::scenegraph.sortLights(frustum);
  //------------------------------------------------------
}



void SceneNamespace::drawBackground()
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
  Render::ren.active_shader->setVec3("clearColor", Render::ren.clearColor);

  Render::ren.active_shader->setVec3("ViewPos", Render::ren.cam.m_transform->getPos_worldspace());
  Render::ren.active_shader->setVec3("SunPos", SceneNamespace::scenegraph.dirlight.position);

  GLCALL(glBindVertexArray(Render::ren.quadVAO));
  GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));
}


void SceneNamespace::drawTerrain()
{
  Render::ren.useShader(SHADER_TERRAIN);

  for (auto &obj: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_TERRAIN))
  {
    EntityComponent *tc = obj->entity_components.getComponent(COMPONENT_TERRAIN);
    Render::ren.drawTerrain(obj->m_model, obj->getTransform(), tc->threshold, tc->epsilon);
  }
}


void SceneNamespace::drawStatic()
{
  Render::ren.useShader(SHADER_ACTOR);

  for (auto &obj: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_STATIC))
    Render::ren.drawModel(obj->m_model, obj->getTransform());
}


void SceneNamespace::drawBillboards()
{
  Render::ren.useShader(SHADER_BILLBOARD_FIXED);
  
  for (auto &obj: *SceneNamespace::scenegraph.getVisibleInstancesByType(GAMEOBJECT_BILLBOARD, INSTANCING_OFF))
    Render::ren.drawBillboard(obj->m_model, obj->getTransform());
}

void SceneNamespace::drawBillboardsInstanced()
{
  Render::ren.useShader(SHADER_BILLBOARD_FOLLOW);

  glm::mat4 view_noTranslate = Render::ren.cam.view;
  view_noTranslate[3] = glm::vec4(0.0f, 0.0f, 0.f, 1.0f);
  Render::ren.active_shader->setMat4("view_noTranslate", view_noTranslate);
  Render::ren.active_shader->setInt("diffuseMap", 0);

  std::map<std::string, InstanceData> *map = SceneNamespace::scenegraph.getInstanceData();
  for (auto it = map->begin(); it != map->end(); ++it)
  {
    InstanceData *instance_data = &(it)->second;

    Render::ren.drawModelInstanced(instance_data->model, instance_data);
  }
}



void SceneNamespace::drawActors()
{
  Render::ren.useShader(SHADER_ACTOR);

  Render::ren.active_shader->setInt("material.diffuseMap", 0);
  Render::ren.active_shader->setInt("material.specularMap", 1);
  Render::ren.active_shader->setInt("material.normalMap", 2);
  Render::ren.active_shader->setInt("material.emissionMap", 3);

  for (auto &obj: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_ACTOR))
  {
    Render::ren.active_shader->setVec3("emission", obj->emission);
    Render::ren.active_shader->setFloat("emission_scale", obj->emission_scale);

    Render::ren.drawModel(obj->m_model, obj->getTransform());
  }

  Render::ren.active_shader->setVec3("emission", glm::vec3(0.0f));
  Render::ren.active_shader->setFloat("emission_scale", 0.0f);
}


void SceneNamespace::drawLightsources()
{
  Render::ren.useShader(SHADER_LIGHTSOURCE);

  for (GameObject *obj: *SceneNamespace::scenegraph.getVisibleInstancesByType(GAMEOBJECT_LIGHTSOURCE))
    Render::ren.drawLightSource(obj->m_model, obj->getTransform(), glm::vec3(1.0f));
}


void SceneNamespace::drawGeometry_batched()
{
  SceneNamespace::drawTerrain();
  SceneNamespace::drawStatic();
  SceneNamespace::drawActors();
  SceneNamespace::drawLightsources();

  
  GLCALL( glDisable(GL_CULL_FACE) );
  SceneNamespace::drawBillboards();
  SceneNamespace::drawBillboardsInstanced();
  GLCALL( glEnable(GL_CULL_FACE) );


  if (Render::ren.getDebugData()->getDebugFlag(RenderDebugFlag::DrawBoundingSpheres))
  {
    Render::ren.useShader(SHADER_WIREFRAME);

    Render::ren.active_shader->setMat4("projection", Render::ren.cam.projection);
    Render::ren.active_shader->setMat4("view", Render::ren.cam.view);


    for (GameObject *obj: *SceneNamespace::scenegraph.getVisibleInstancesByType(GAMEOBJECT_TERRAIN))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->m_model, obj->getTransform());

     for (GameObject *obj: *SceneNamespace::scenegraph.getVisibleInstancesByType(GAMEOBJECT_STATIC))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->m_model, obj->getTransform());   

    for (GameObject *obj: *SceneNamespace::scenegraph.getVisibleInstancesByType(GAMEOBJECT_ACTOR))
      Render::ren.drawPrimitive(PRIMITIVE_SPHERE, obj->m_model, obj->getTransform());
  }

}

void SceneNamespace::drawGeometry()
{
  for (auto &object: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_TERRAIN))
    Render::ren.drawModel(object->m_model, object->getTransform());
   
  for (auto &object: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_STATIC))
    Render::ren.drawModel(object->m_model, object->getTransform());

  for (auto &object: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_ACTOR))
    Render::ren.drawModel(object->m_model, object->getTransform());

  for (auto &object: *SceneNamespace::scenegraph.getInstancesByType(GAMEOBJECT_BILLBOARD, INSTANCING_OFF))
    Render::ren.drawModel(object->m_model, object->getTransform());
}



