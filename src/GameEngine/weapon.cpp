#include "weapon.h"

void lerp(glm::vec3 *from, glm::vec3 to, float alpha)
{
  glm::vec3 direc = to - *from;
  float dist = glm::length(direc);
  direc *= alpha;
  *from += direc;
}

void Weapon::load_model(const char *filepath)
{
  this->model.load(filepath);
}

void Weapon::draw(void)
{
  renderer.cam.projection = glm::perspective(glm::radians(renderer.fov), (float)SCR_WDTH / (float)SCR_HGHT, 0.001f, RENDER_DISTANCE);
  glm::mat4 backup_view = renderer.cam.view;
  renderer.cam.view = glm::mat4(1.0f);

  this->movement_offset -= (1.0f / this->sway) * this->movement_offset; 


  float x_offset = 0.001f * sin(renderer.cam.headbob_value);
  float y_offset = 0.001f * cos(renderer.cam.headbob_value);


  if (this->aiming)
    lerp(&this->model.pos, this->aim_pos + this->movement_offset, 0.055f);
  else
    lerp(&this->model.pos, this->hip_pos + this->movement_offset, 0.055f);

  this->model.draw();

  renderer.cam.view = backup_view;
  renderer.cam.projection = glm::perspective(glm::radians(renderer.fov), (float)SCR_WDTH / (float)SCR_HGHT, NEAR_PLANE_DIST, RENDER_DISTANCE);
}

