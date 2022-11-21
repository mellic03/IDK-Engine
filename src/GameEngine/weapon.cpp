#include "weapon.h"

glm::vec4 lerp(glm::vec3 from, glm::vec3 to, float alpha)
{
  glm::vec3 direc = to - from;
  float dist = glm::length(direc);
  direc *= alpha;
  glm::vec3 result = from + direc;
  return glm::vec4(result.x, result.y, result.z, 1.0f);
}

void Weapon::load_model(const char *filepath)
{
  this->model.load(filepath);
}

void Weapon::draw(void)
{
  renderer.cam.projection = glm::perspective(glm::radians(renderer.fov), (float)renderer.SCR_width/(float)renderer.SCR_height, 0.001f, RENDER_DISTANCE);
  glm::mat4 backup_view = renderer.cam.view;
  renderer.cam.view = glm::mat4(1.0f);

  this->movement_offset -= (1.0f / this->sway) * this->movement_offset; 


  float x_offset = 0.001f * sin(renderer.cam.headbob_value);
  float y_offset = 0.001f * cos(renderer.cam.headbob_value);


  if (this->aiming)
    this->model.pos = lerp(glm::vec3(this->model.pos), this->aim_pos + this->movement_offset, 0.055f);
  else
    this->model.pos = lerp(glm::vec3(this->model.pos), this->hip_pos + this->movement_offset, 0.055f);

  this->model.draw();

  renderer.cam.view = backup_view;
  renderer.cam.projection = glm::perspective(glm::radians(renderer.fov), (float)renderer.SCR_width/(float)renderer.SCR_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
}

