#include "model.h"
#include "../GameEngine/physics.h"

bool Model::load(std::string filepath)
{
  std::string assetfilepath = filepath + "asset.txt";

  FILE *fh = fopen(assetfilepath.c_str(), "r");
  if (fh == NULL)
  {
    printf("Error opening %s\n", assetfilepath);
    return false;
  }

  char buffer[64];
  char stringdata[64];
  int intdata;

  while (fgets(buffer, 64, fh) != NULL)
  {
    sscanf(buffer, "%s", stringdata);

    if (strcmp(stringdata, "#ANIMATED") == 0)
    {
      this->is_animated = true;
    }

    else if (strcmp(stringdata, "#ENVIRONMENTAL") == 0)
    {
      this->is_environmental = true;
    }
  }

  rewind(fh);

  bool collisionmesh_loaded = false;

  while (fgets(buffer, 64, fh) != NULL)
  {
    if (sscanf(buffer, "ANIMATION REST %s %d", stringdata, &intdata))
    {
      // printf("animation: %s, keyframes: %d\n", stringdata, intdata);
      this->animations[ANIM_REST].loadKeyframes(filepath, stringdata, intdata);
      this->animations[ANIM_REST].setPos(this->position);
      this->animations[ANIM_REST].setRot(this->rotation);
    }

    else if (sscanf(buffer, "GEOMETRYMESH %s", stringdata))
    {
      // printf("static: %s\n", stringdata);
      this->m_staticmesh.load(filepath.c_str(), stringdata);
    }

    else if (sscanf(buffer, "COLLISIONMESH %s", stringdata))
    {
      // printf("static: %s\n", stringdata);
      this->m_collision_mesh.load(filepath.c_str(), stringdata);
      collisionmesh_loaded = true;
    }

    else if (sscanf(buffer, "NAVMESH %s", stringdata))
    {
      // printf("navmesh: %s\n", stringdata);
    }
  }

  if (collisionmesh_loaded == false)
    this->m_collision_mesh = this->m_staticmesh;

  fclose(fh);

  return true;
}


void Model::activeAnimation(AnimationType id)
{
  this->m_active_animation = id;
}


void Model::drawStaticMesh(Renderer *ren)
{
  this->m_staticmesh.draw(ren);
}

void Model::drawAnimatedMesh(Renderer *ren)
{
  switch (this->m_state)
  {
    case (MSTATE_NOANIM_PLAYING):
      this->animations[ANIM_REST].play(ren);
      break;

    case (MSTATE_ANIM_PLAYING):
      this->animations[this->m_active_animation].play(ren);
      break;
  }
}

void Model::setPos(glm::vec3 *position)
{
  this->position = position;

  this->m_staticmesh.setPos(position);

  for (int i=0; i<NUM_ANIMATION_TYPES; i++)
    this->animations[i].setPos(position);
}


void Model::setRot(glm::vec3 *rotation)
{
  this->rotation = rotation;

  this->m_staticmesh.setRot(rotation);

  for (int i=0; i<NUM_ANIMATION_TYPES; i++)
    this->animations[i].setRot(rotation);
}


void Model::draw(Renderer *ren)
{
  (this->is_animated) ? this->drawAnimatedMesh(ren) : this->drawStaticMesh(ren);
}


void Model::collideWithPlayer(Player *player)
{
  float nearest_dist = INFINITY;
  int nearest_i = 0;

  glm::vec3 ray_up    = glm::vec3( 0.0f, +1.0f,  0.0f);
  glm::vec3 ray_down  = glm::vec3( 0.0f, -1.0f,  0.0f);
  glm::vec3 ray_left  = -player->cam->right;
  glm::vec3 ray_right =  player->cam->right;
  glm::vec3 ray_front =  player->cam->front;
  glm::vec3 ray_back  = -player->cam->front;

  glm::vec3 ray_traveldir = glm::normalize(player->vel);

  glm::mat4 inverse_model = glm::inverse(this->m_staticmesh.model_mat);

  for (int i=0; i<this->m_collision_mesh.num_vertices; i+=3)
  {

    glm::vec3 v0 = this->m_collision_mesh.vertices[i+0].position * glm::mat3(inverse_model);
    glm::vec3 v1 = this->m_collision_mesh.vertices[i+1].position * glm::mat3(inverse_model);
    glm::vec3 v2 = this->m_collision_mesh.vertices[i+2].position * glm::mat3(inverse_model);
    glm::vec3 normal = glm::vec4(
      this->m_collision_mesh.vertices[i+2].face_normal.x,
      this->m_collision_mesh.vertices[i+2].face_normal.y,
      this->m_collision_mesh.vertices[i+2].face_normal.z,
      0.0f      
    ) * inverse_model;



    player->temp_pos = glm::vec3(player->pos->x, player->pos->y, player->pos->z);

    // Find nearest "down"
    if (glm::dot(ray_down, normal) <= 0)
    {
      glm::vec3 intersect_point;
      bool intersects = ray_intersect_triangle(player->temp_pos, ray_down, v0, v1, v2, &intersect_point);
      if (intersects)
      {
        float dist = glm::distance(player->temp_pos, intersect_point);
        if (dist < nearest_dist)
        {
          nearest_dist = dist;
          nearest_i = i;
        }
      }
    }

    if (glm::dot(ray_up, normal) <= 0)
      player_collide(player, ray_up   , v0, v1, v2, normal, player->height/2, false);

    if (glm::dot(ray_left, normal) <= 0)
      player_collide(player, ray_left , v0, v1, v2, normal, player->height/2, false);

    if (glm::dot(ray_right, normal) <= 0)
      player_collide(player, ray_right, v0, v1, v2, normal, player->height/2, false);

    if (glm::dot(ray_front, normal) <= 0)
      player_collide(player, ray_front, v0, v1, v2, normal, player->height/2, false);

    if (glm::dot(ray_back, normal) <= 0)
      player_collide(player, ray_back , v0, v1, v2, normal, player->height/2, false);

    player_collide(player, ray_traveldir , v0, v1, v2, normal, player->height/2, false);
    
  }

  glm::vec3 v0 = this->m_collision_mesh.vertices[nearest_i+0].position + *this->position;
  glm::vec3 v1 = this->m_collision_mesh.vertices[nearest_i+1].position + *this->position;
  glm::vec3 v2 = this->m_collision_mesh.vertices[nearest_i+2].position + *this->position;
  glm::vec3 normal = this->m_collision_mesh.vertices[nearest_i+2].face_normal;

  player_collide(player, ray_down, v0, v1, v2, normal, player->height, true);

}