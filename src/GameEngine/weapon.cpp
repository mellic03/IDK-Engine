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
  REN_active_cam.projection = glm::perspective(glm::radians(REN_fov), (float)SCR_WDTH / (float)SCR_HGHT, 0.001f, RENDER_DISTANCE);
  glm::mat4 backup_view = REN_active_cam.view;
  REN_active_cam.view = glm::mat4(1.0f);

  this->movement_offset -= (1.0f / this->sway) * this->movement_offset; 


  float x_offset = 0.001f * sin(REN_active_cam.headbob_value);
  float y_offset = 0.001f * cos(REN_active_cam.headbob_value);


  if (this->aiming)
    lerp(&this->model.pos, this->aim_pos + this->movement_offset, 0.055f);
  else
    lerp(&this->model.pos, this->hip_pos + this->movement_offset, 0.055f);

  this->model.draw();

  REN_active_cam.view = backup_view;
  REN_active_cam.projection = glm::perspective(glm::radians(REN_fov), (float)SCR_WDTH / (float)SCR_HGHT, NEAR_PLANE_DIST, RENDER_DISTANCE);
}

