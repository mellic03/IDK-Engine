#include "physics.h"





void ModelContainer::add(Model *model)
{
  this->models.push_back(model);
}

void ModelContainer::draw(Renderer *ren)
{
  for (int i=0; i<this->models.size(); i++)
  {
    this->models[i]->draw(ren);
  }
}

bool ray_intersect_triangle(  glm::vec3 ray_pos, glm::vec3 ray_dir,
                              glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                              glm::vec3 *intersect_point  )
{
  const float EPSILON = 0.0000001;
  glm::vec3 edge1, edge2, h, s, q;
  float a,f,u,v;
  edge1 = v1 - v0;
  edge2 = v2 - v0;

  h = glm::cross(ray_dir, edge2);
  a = glm::dot(edge1, h);
  if (a > -EPSILON && a < EPSILON)
    return false;

  f = 1.0/a;
  s = ray_pos - v0;
  u = f * glm::dot(s, h);
  if (u < 0.0 || u > 1.0)
    return false;

  q = glm::cross(s, edge1);
  v = f * glm::dot(ray_dir, q);
  if (v < 0.0 || u + v > 1.0)
    return false;

  float t = f * glm::dot(edge2, q);
  if (t > EPSILON)
  {
    *intersect_point = ray_pos + ray_dir * t;
    return true;
  }
  else
    return false;
}


float calculate_impulse(glm::vec3 vel, glm::vec3 face_normal, float mass)
{
  float relative_to_normal = glm::dot(vel, face_normal);
  if (relative_to_normal > 0)
    return 0.0f;
  
  float j = -1 * relative_to_normal;
  j /= (1/mass) + 0.0001;

  return j;
}


void player_collide(Player *player, glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards)
{
  glm::vec3 intersect_point;

  bool intersects = ray_intersect_triangle(player->temp_pos, ray, v0, v1, v2, &intersect_point);


  if (intersects)
  {
    float dist = glm::distance(player->temp_pos, intersect_point);

    if (0 < dist && dist < d)
    {
      float impulse_1d = calculate_impulse(player->vel, normal, 0.1);
      glm::vec3 impulse = impulse_1d * normal;

      if (downwards)
      {
        float overlap = d - dist;
        player->pos->y += overlap;
        player->vel.y = 0;
        player->changeState(PSTATE_GROUNDED);
      }

      else
      {
        player->vel += (1.0f) * impulse;
        *player->pos = *player->pos - (d-dist)*ray;
      }
    }

    else if (dist > d && downwards)
      player->changeState(PSTATE_FALLING);

  }
}



int tests_run = 0;
float total_time = 0.0f;

void ModelContainer::collide(Player *player)
{
  clock_t time = clock();

  float nearest_dist = INFINITY;
  int nearest_i = 0;
  int nearest_j = 0;

  glm::vec3 ray_up    = glm::vec3( 0.0f, +1.0f,  0.0f);
  glm::vec3 ray_down  = glm::vec3( 0.0f, -1.0f,  0.0f);
  glm::vec3 ray_left  = -player->cam->right;
  glm::vec3 ray_right =  player->cam->right;
  glm::vec3 ray_front =  player->cam->front;
  glm::vec3 ray_back  = -player->cam->front;

  for (int i=0; i<this->models.size(); i++)
  {
    for (int j=0; j<this->models[i]->num_vertices; j+=3)
    {

      glm::vec3 v0 = this->models[i]->vertices[j+0].position + this->models[i]->pos;
      glm::vec3 v1 = this->models[i]->vertices[j+1].position + this->models[i]->pos;
      glm::vec3 v2 = this->models[i]->vertices[j+2].position + this->models[i]->pos;
      glm::vec3 normal = this->models[i]->vertices[j+2].face_normal;

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
            nearest_j = j;
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

    }
  }

  glm::vec3 v0 = this->models[nearest_i]->vertices[nearest_j+0].position + this->models[nearest_i]->pos;
  glm::vec3 v1 = this->models[nearest_i]->vertices[nearest_j+1].position + this->models[nearest_i]->pos;
  glm::vec3 v2 = this->models[nearest_i]->vertices[nearest_j+2].position + this->models[nearest_i]->pos;
  glm::vec3 normal = this->models[nearest_i]->vertices[nearest_j+2].face_normal;

  player_collide(player, ray_down, v0, v1, v2, normal, player->height, true);


  tests_run += 1;

  total_time += clock() - time;
  // printf("average time: %f\n", (double)(total_time/tests_run) / CLOCKS_PER_SEC);

}


void ModelContainer::bindRenderer(Renderer *ren)
{
  for (int i=0; i<this->models.size(); i++)
  {
    this->models[i]->view_mat = &ren->cam.view;
    this->models[i]->projection_mat = &ren->cam.projection;
  }
}

