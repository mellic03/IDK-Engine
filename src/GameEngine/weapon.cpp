#include "weapon.h"

glm::vec3 lerp(glm::vec3 from, glm::vec3 to, float alpha)
{
  glm::vec3 direc = to - from;
  float dist = glm::length(direc);
  direc *= alpha;
  return from + direc;
}

void Weapon::load_model(const char *filepath, const char *name, Renderer *ren)
{
  this->model.load(filepath, name);
  this->model.bindRenderer(ren);
}

void Weapon::draw(Renderer *ren)
{
  ren->cam.projection = glm::perspective(glm::radians(ren->cam.fov), (float)ren->SCR_width/(float)ren->SCR_height, 0.001f, RENDER_DISTANCE);

  this->movement_offset -= (1.0f / this->sway) * this->movement_offset; 
  this->movement_offset = glm::clamp(this->movement_offset, glm::vec3(-0.2, -0.1, 0.0), glm::vec3(0.05, 0.1, 0.0));


  float x_offset = 0.001f * sin(ren->cam.headbob_value);
  float y_offset = 0.001f * cos(ren->cam.headbob_value);


  if (this->aiming)
    *this->model.pos = lerp(glm::vec3(*this->model.pos), this->aim_pos + this->movement_offset, 0.15f);
  else
    *this->model.pos = lerp(glm::vec3(*this->model.pos), this->hip_pos + this->movement_offset, 0.15f);
  
  
  this->model.set_pos(*this->model.pos);
  

  this->model.draw(ren);

  ren->cam.projection = glm::perspective(glm::radians(ren->cam.fov), (float)ren->SCR_width/(float)ren->SCR_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
}

