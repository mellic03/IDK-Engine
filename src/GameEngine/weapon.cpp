#include "weapon.h"

glm::vec3 lerp(glm::vec3 from, glm::vec3 to, float alpha)
{
  glm::vec3 direc = to - from;
  float dist = glm::length(direc);
  direc *= alpha;
  return from + direc;
}

void Weapon::load_model(const char *filepath, Camera *cam)
{
  this->model.load(filepath);
  this->model.setShader(cam->shaders[SHADER_VIEWSPACE]);
}

void Weapon::draw(Camera *cam)
{
  cam->projection = glm::perspective(glm::radians(cam->fov), (float)cam->SCR_width/(float)cam->SCR_height, 0.001f, RENDER_DISTANCE);

  this->movement_offset -= (1.0f / this->sway) * this->movement_offset; 


  float x_offset = 0.001f * sin(cam->headbob_value);
  float y_offset = 0.001f * cos(cam->headbob_value);


  if (this->aiming)
    this->model.pos = lerp(glm::vec3(this->model.pos), this->aim_pos + this->movement_offset, 0.055f);
  else
    this->model.pos = lerp(glm::vec3(this->model.pos), this->hip_pos + this->movement_offset, 0.055f);
  
  this->model.set_pos(this->model.pos);

  this->model.draw(cam);

  cam->projection = glm::perspective(glm::radians(cam->fov), (float)cam->SCR_width/(float)cam->SCR_height, NEAR_PLANE_DIST, RENDER_DISTANCE);
}

