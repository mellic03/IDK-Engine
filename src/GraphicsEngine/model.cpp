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

  bool name_supplied = false;

  while (fgets(buffer, 64, fh) != NULL)
  {
    sscanf(buffer, "%s", stringdata);

    if (strcmp(stringdata, "#ANIMATED") == 0)
      this->is_animated = true;

    else if (strcmp(stringdata, "#ENVIRONMENTAL") == 0)
      this->is_environmental = true;

    else if (strcmp(stringdata, "#NPC") == 0)
      this->is_NPC = true;

    if (sscanf(buffer, "#ASSETNAME %s", stringdata))
    {
      this->m_name = std::string(stringdata);
      name_supplied = true;
    }
  }

  if (name_supplied == false)
  {
    printf("Name not supplied for asset: %s\n", filepath);
    exit(1);
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
  
  // printf("position.y: %f\n", position->y);
  // if (position->y < 0.0f) exit(1);

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

  glm::mat4 model = (this->m_staticmesh.model_mat);
  
  glm::vec3 ray_up    =  glm::vec3( 0.0f, +1.0f,  0.0f); 
  glm::vec3 ray_down  =  glm::vec3( 0.0f, -1.0f,  0.0f);
  glm::vec3 ray_left  = -player->cam->right;
  glm::vec3 ray_right =  player->cam->right;
  glm::vec3 ray_front =  player->cam->front;
  glm::vec3 ray_back  = -player->cam->front;

  glm::vec3 ray_traveldir = glm::normalize(player->vel);


  for (int i=0; i<this->m_collision_mesh.num_vertices; i+=3)
  {

    glm::vec3 v0 = (this->m_collision_mesh.vertices[i+0].position);
    glm::vec3 v1 = (this->m_collision_mesh.vertices[i+1].position);
    glm::vec3 v2 = (this->m_collision_mesh.vertices[i+2].position);

    v0 = model * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
    v1 = model * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
    v2 = model * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

    glm::normalize(v0);
    glm::normalize(v1);
    glm::normalize(v2);

    glm::vec3 normal = glm::mat3(model) * this->m_collision_mesh.vertices[i+2].face_normal;
    glm::normalize(normal);


    // Find nearest "down"
    if (glm::dot(ray_down, normal) <= 0)
    {
      glm::vec3 intersect_point;
      bool intersects = ray_intersect_triangle(*player->pos, ray_down, v0, v1, v2, &intersect_point);
      if (intersects)
      {
        float dist = glm::distance(*player->pos, intersect_point);
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

  glm::vec3 v0 = this->m_collision_mesh.vertices[nearest_i+0].position;
  glm::vec3 v1 = this->m_collision_mesh.vertices[nearest_i+1].position;
  glm::vec3 v2 = this->m_collision_mesh.vertices[nearest_i+2].position;
  v0 = model * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
  v1 = model * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
  v2 = model * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

  glm::vec3 normal = glm::mat3(model) * this->m_collision_mesh.vertices[nearest_i+2].face_normal;


  player_collide(player, ray_down, v0, v1, v2, normal, player->height, true);

}