#include "physics.h"

#define Sq(n) ((n)*(n))

using namespace glm;

bool PhysicsEngine::ray_intersect_triangle(vec3 ray_pos, vec3 ray_dir, vec3 v0, vec3 v1, vec3 v2, vec3 *intersect_point)
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


float PhysicsEngine::calculate_impulse(glm::vec3 vel, glm::vec3 face_normal, float mass)
{
  float relative_to_normal = glm::dot(vel, face_normal);
  if (relative_to_normal > 0)
    return 0.0f;
  
  float j = -1 * relative_to_normal;
  j /= (1/mass) + 0.0001;

  return j;
}


glm::vec3 PhysicsEngine::triangle_normal(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
  return glm::normalize(glm::cross(B - A, C - A));
}


bool PhysicsEngine::line_intersects_sphere(glm::vec3 A, glm::vec3 B, glm::vec3 P, float radius, glm::vec3 *dir)
{
  float n1 = (P.x - A.x)*(B.x - A.x);
  float n2 = (P.y - A.y)*(B.y - A.y);
  float n3 = (P.z - A.z)*(B.z - A.z);

  float d1 = Sq(B.x - A.x);
  float d2 = Sq(B.y - A.y);
  float d3 = Sq(B.z - A.z);

  float u = (n1 + n2 + n3) / (d1 + d2 + d3);
  
  if (u < 0 || u > 1)
    return false;

  float x = A.x + u*(B.x - A.x);
  float y = A.y + u*(B.y - A.y);
  float z = A.z + u*(B.z - A.z);

  *dir = glm::normalize(P - glm::vec3(x, y, z));

  return glm::distance(P, glm::vec3(x, y, z)) < radius;

  return false;
}


bool PhysicsEngine::sphere_triangle_detect(GameObject *obj, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir)
{
  *plane_dist = glm::dot(v0.normal, *obj->getPos() - v0.position);
  
  // If center of sphere is closer to plane than its radius
  if (fabs(*plane_dist) <= obj->m_sphere_collider_radius)
  {
    glm::vec3 closest_point = *obj->getPos() + (*plane_dist * -v0.normal);
    glm::vec3 N1 = triangle_normal(v0.position, v1.position, closest_point);
    glm::vec3 N2 = triangle_normal(v1.position, v2.position, closest_point);
    glm::vec3 N3 = triangle_normal(v2.position, v0.position, closest_point);

    if (glm::dot(N1, N2) > 0.9999f && glm::dot(N2, N3) > 0.9999f)
      return true;


    // check if any edges intersect sphere
    if (line_intersects_sphere(v0.position, v1.position, *obj->getPos(), obj->m_sphere_collider_radius, dir))
    {
      *edge_collision = true;
      return true;
    }

    if (line_intersects_sphere(v1.position, v2.position, *obj->getPos(), obj->m_sphere_collider_radius, dir))
    {
      *edge_collision = true;
      return true;
    }

    if (line_intersects_sphere(v2.position, v0.position, *obj->getPos(), obj->m_sphere_collider_radius, dir))
    {
      *edge_collision = true;
      return true;
    }
  }

  return false;
}


void PhysicsEngine::sphere_triangle_response(GameObject *obj1, Vertex v0, Vertex v1, Vertex v2, float dist, bool edge_collision, glm::vec3 dir)
{
  float overlap = obj1->m_sphere_collider_radius - dist;
  
  float impulse_1d = 0.0f;

  if (edge_collision)
  {
    overlap = 0.0f;
    impulse_1d = calculate_impulse(*obj1->getVel(), dir, 1.0f);
  }

  else
    impulse_1d = calculate_impulse(*obj1->getVel(), v0.normal, 1.0f);


  if (impulse_1d == 0.0f)
    return;

  glm::vec3 impulse = v0.normal * impulse_1d;
  obj1->getVel()->x += 1.0f/1.0f * impulse.x;
  obj1->getVel()->y += 1.0f/1.0f * impulse.y;
  obj1->getVel()->z += 1.0f/1.0f * impulse.z;

  *obj1->getPos() += 0.5f * overlap * v0.normal;
}


bool PhysicsEngine::capsule_triangle_detect(GameObject *obj, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir)
{
  
}


