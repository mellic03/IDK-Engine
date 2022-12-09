#include "physics.h"
#include "gameobject.h"



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

  bool intersects = ray_intersect_triangle(*player->pos, ray, v0, v1, v2, &intersect_point);


  if (intersects)
  {
    float dist = glm::distance(*player->pos, intersect_point);

    if (0 < dist && dist < d)
    {
      float impulse_1d = calculate_impulse(player->vel, normal, 0.5);
      glm::vec3 impulse = impulse_1d * normal;

      player->vel += (1.0f) * impulse;
      if (downwards)
      {
        // float overlap = d - dist;
        // player->pos->y += overlap;
        // player->vel.y = 0;
        player->changeState(PSTATE_GROUNDED);
      }

      else
      {
        *player->pos = *player->pos - (d-dist)*ray;
      }
    }

    else if (dist > d && downwards)
      player->changeState(PSTATE_FALLING);
  }
}




