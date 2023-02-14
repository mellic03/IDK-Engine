#include "GameEngine.h"



void Engine::init(Renderer *ren)
{
  ren->init();

  AudioEngine::init();

  Scene::init();
  Scene::scenegraph.loadObjects("assets/index.txt");
  Scene::defaultScene();
  Scene::importScene("assets/scenes/entry.scene");

  luaInit();
}


void Engine::newFrame(Renderer *ren, int *x, int *y, int *w, int *h)
{
  #ifndef IDK_SHIPPING
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    draw_ui(ren, x, y, w, h);
  #else
    SDL_GetWindowSize(window, &w, &h);
  #endif
}


void Engine::endFrame(SDL_Window *window, Renderer *ren, Uint64 dTime_start, Uint64 dTime_end)
{
  #ifndef IDK_SHIPPING
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  #endif

  SDL_GL_SwapWindow(window);
  double dtime_milliseconds = ((dTime_end - dTime_start)*1000 / (double)SDL_GetPerformanceFrequency() );
  ren->deltaTime = dtime_milliseconds / 1000;
}




void Engine::geometryPass(Renderer *ren)
{
  GLCALL( glViewport(0, 0, ren->viewport_width, ren->viewport_height) );
  GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, ren->gbufferFBO) );
  GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
  GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
  Scene::drawBackground();
  Scene::drawGeometry_batched();
}


void Engine::lightingPass(Renderer *ren)
{
  GLCALL(glViewport(0, 0, ren->viewport_width, ren->viewport_height));
  glBindFramebuffer(GL_FRAMEBUFFER, ren->colorFBO);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  ren->useShader(SHADER_GBUFFER_LIGHTING);
  Scene::sendLightsToShader();

  ren->active_shader->setMat4("view", ren->cam.view);
  ren->active_shader->setMat4("projection", ren->cam.projection);

  for (int i=0; i<NUM_SHADOW_CASCADES; i++)
  {
    std::string stringi = std::to_string(i);

    ren->active_shader->setFloat("shadow_cascades[" + stringi + "]", ren->cascaded_rsm.cascade_distances[i]);
    ren->active_shader->setMat4("dir_lightSpaceMatrices[" + stringi + "]", ren->cascaded_rsm.lightSpaceMatrices[i]);

    glActiveTexture(GL_TEXTURE5 + i);
    glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.depthArray[i]);
    ren->active_shader->setInt("dir_depthmaps[" + stringi + "]", 5 + i);
  }


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ren->gbuffer_position);
  ren->active_shader->setInt("gPosition", 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, ren->gbuffer_normal);
  ren->active_shader->setInt("gNormal", 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, ren->gbuffer_albedospec);
  ren->active_shader->setInt("gAlbedoSpec", 2);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, ren->gbuffer_emission);
  ren->active_shader->setInt("gEmission", 3);

  glBindVertexArray(ren->quadVAO);

  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);


  glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Engine::volumetricLightPass(Renderer *ren)
{
  glViewport(0, 0, ren->viewport_width/ren->volumetrics.resolution_divisor, ren->viewport_height/ren->volumetrics.resolution_divisor);
  glBindFramebuffer(GL_FRAMEBUFFER, ren->lightshaftFBO);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ren->useShader(SHADER_VOLUMETRIC_LIGHT);

  ren->active_shader->setMat4("projection", ren->cam.projection);
  ren->active_shader->setMat4("view", ren->cam.view);
  for (int i=0; i<NUM_SHADOW_CASCADES; i++)
  {
    ren->active_shader->setFloat("shadow_cascades[" + std::to_string(i) + "]", ren->cascaded_rsm.cascade_distances[i]);
    ren->active_shader->setMat4("dir_lightSpaceMatrices[" + std::to_string(i) + "]", ren->cascaded_rsm.lightSpaceMatrices[i]);

    glActiveTexture(GL_TEXTURE5 + i);
    glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.depthArray[i]);
    ren->active_shader->setInt("dir_depthmaps[" + std::to_string(i) + "]", 5 + i);
  }

  glActiveTexture(GL_TEXTURE12);
  glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.positionArray[2]);
  ren->active_shader->setInt("dir_positionArraytest", 12);

  glActiveTexture(GL_TEXTURE13);
  glBindTexture(GL_TEXTURE_2D, ren->cascaded_rsm.normalArray[2]);
  ren->active_shader->setInt("dir_normalArraytest", 13);




  Scene::sendLightsToShader();
  Scene::sendVolumetricData();
  ren->active_shader->setVec3("viewDir", ren->cam.front);
  
  glBindVertexArray(ren->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glBindTexture(GL_TEXTURE_2D, ren->lightshaftColorBuffer);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Engine::combinePass(Renderer *ren)
{
  GLCALL( glViewport(0, 0, ren->viewport_width, ren->viewport_height) );
  GLCALL( glDisable(GL_DEPTH_TEST) );

  ren->useShader(SHADER_SCREENQUAD);
  ren->postProcess();
  
  glBindFramebuffer(GL_FRAMEBUFFER, ren->generalFBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_2D, ren->colorBuffer);

  glActiveTexture(GL_TEXTURE11);
  glBindTexture(GL_TEXTURE_2D, ren->lightshaftColorBuffer);

  ren->active_shader->setInt("screenTexture", 10);
  ren->active_shader->setInt("volumetricLightsTexture", 11);
  ren->active_shader->setFloat("bloom_threshold", ren->bloom.threshold);

  
  glBindVertexArray(ren->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  ren->blurTexture(ren->generalColorBuffers[1], ren->billboardFBO);
}


void Engine::fxaa(Renderer *ren)
{
  GLCALL( glViewport(0, 0, ren->viewport_width, ren->viewport_height) );

  ren->useShader(SHADER_FXAA);

  #ifndef IDK_SHIPPING
    glBindFramebuffer(GL_FRAMEBUFFER, ren->screenQuadFBO);
  #else
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  #endif

  GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );

  GLCALL( glActiveTexture(GL_TEXTURE10) );
  glBindTexture(GL_TEXTURE_2D, ren->generalColorBuffers[0]);
  ren->active_shader->setInt("screenTexture", 10);

  glActiveTexture(GL_TEXTURE11);
  glBindTexture(GL_TEXTURE_2D, ren->billboardColorBuffer);
  ren->active_shader->setInt("bloomTexture", 11);

  ren->active_shader->setFloat("bloomAmount", ren->bloom.strength);

  glBindVertexArray(ren->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Engine::renderScene(Renderer *ren)
{
  Scene::perFrameUpdate();
  Scene::drawDepthmaps();
  
  Engine::geometryPass(ren);
  Engine::lightingPass(ren);
  Engine::volumetricLightPass(ren);
  Engine::combinePass(ren);
  Engine::fxaa(ren);
}