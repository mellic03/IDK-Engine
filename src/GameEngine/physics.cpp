#include "physics.h"

#define Sq(n) ((n)*(n))

using namespace glm;

float PhysicsEngine::gravity = 1.0f;


bool PhysicsEngine::ray_intersects_plane(glm::vec3 ray_origin, glm::vec3 ray_dir, glm::vec3 plane_normal, glm::vec3 plane_pos)
{
  // float t = -10000000000.0f;
  
  // float dt = glm::dot(ray_dir, plane_normal);

  // if (dt < -0.001 || dt > 0.001)
  //   return true;  
  // t = dot(plane_normal, plane_pos - ray_origin);
  return false;
}


bool PhysicsEngine::ray_intersects_triangle(vec3 ray_pos, vec3 ray_dir, vec3 v0, vec3 v1, vec3 v2, vec3 *intersect_point)
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
}


float point_plane_dist(glm::vec3 point, glm::vec3 plane_normal, glm::vec3 plane_pos)
{
  return glm::dot(plane_normal, point - plane_pos);
} 


bool point_in_triangle(glm::vec3 point, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{ 
  glm::vec3 plane_normal = PhysicsEngine::triangle_normal(v0, v1, v2);

  float plane_dist = point_plane_dist(point, plane_normal, v0);
  glm::vec3 closest_point = point + (plane_dist * -plane_normal);
  glm::vec3 N1 = PhysicsEngine::triangle_normal(v0, v1, closest_point);
  glm::vec3 N2 = PhysicsEngine::triangle_normal(v1, v2, closest_point);
  glm::vec3 N3 = PhysicsEngine::triangle_normal(v2, v0, closest_point);

  return (glm::dot(N1, N2) > 0.9999f && glm::dot(N2, N3) > 0.9999f);
}


bool PhysicsEngine::sphere_triangle_detect(SphereCollider *spherecollider, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir)
{
  *plane_dist = point_plane_dist(spherecollider->pos, v0.normal, v0.position);
  
  // If center of sphere is closer to plane than its radius
  if (fabs(*plane_dist) <= spherecollider->radius)
  {
    if (point_in_triangle(spherecollider->pos, v0.position, v1.position, v2.position))
      return true;

    // check if any edges intersect sphere
    if (line_intersects_sphere(v0.position, v1.position, spherecollider->pos, spherecollider->radius, dir))
    {
      *edge_collision = true;
      return true;
    }

    if (line_intersects_sphere(v1.position, v2.position, spherecollider->pos, spherecollider->radius, dir))
    {
      *edge_collision = true;
      return true;
    }

    if (line_intersects_sphere(v2.position, v0.position, spherecollider->pos, spherecollider->radius, dir))
    {
      *edge_collision = true;
      return true;
    }
  }

  return false;
}


void PhysicsEngine::sphere_triangle_response(SphereCollider *spherecollider, Vertex v0, Vertex v1, Vertex v2, float dist, bool edge_collision, glm::vec3 dir)
{
  float overlap =  spherecollider->radius - dist;
  
  float impulse_1d = 0.0f;

  glm::vec3 tnormal = triangle_normal(v0.position, v1.position, v2.position);

  if (edge_collision)
  {
    overlap = 0.0f;
    impulse_1d = calculate_impulse(spherecollider->vel, dir, 1.0f);
  }


  else
    impulse_1d = calculate_impulse(spherecollider->vel, tnormal, 1.0f);


  if (impulse_1d == 0.0f)
    return;

  glm::vec3 impulse = tnormal * impulse_1d;
  spherecollider->vel.x += 1.0f/1.0f * impulse.x;
  spherecollider->vel.y += 1.0f/1.0f * impulse.y;
  spherecollider->vel.z += 1.0f/1.0f * impulse.z;

  spherecollider->pos += 0.5f * overlap * tnormal;
}


glm::vec3 ClosestPointOnLineSegment(glm::vec3 A, glm::vec3 B, glm::vec3 Point)
{
  glm::vec3 AB = B - A;
  float t = glm::dot(Point - A, AB) / dot(AB, AB);
  return A + glm::min(glm::max(t, 0.0f), 1.0f) * AB;
}


bool PhysicsEngine::capsule_triangle_detect(CapsuleCollider *capsule, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir)
{  
  // Find closest point on line segment to triangle
  glm::vec3 tip  = capsule->pos + glm::vec3(0.0f, capsule->top - capsule->radius, 0.0f);
  glm::vec3 base = capsule->pos - glm::vec3(0.0f, capsule->bottom - capsule->radius, 0.0f);


  SphereCollider sphere1;
  sphere1.pos = tip;
  sphere1.vel = capsule->vel;
  sphere1.radius = capsule->radius;

  if (PhysicsEngine::sphere_triangle_detect(&sphere1, v0, v1, v2, plane_dist, edge_collision, dir))
    PhysicsEngine::sphere_triangle_response(&sphere1, v0, v1, v2, *plane_dist, *edge_collision, *dir);

  capsule->pos += sphere1.pos - tip;
  capsule->vel += sphere1.vel - capsule->vel;


  SphereCollider sphere2;
  sphere2.pos = base;
  sphere2.vel = capsule->vel;
  sphere2.radius = capsule->radius;

  if (PhysicsEngine::sphere_triangle_detect(&sphere2, v0, v1, v2, plane_dist, edge_collision, dir))
    PhysicsEngine::sphere_triangle_response(&sphere2, v0, v1, v2, *plane_dist, *edge_collision, *dir);

  capsule->pos += sphere2.pos - base;
  capsule->vel += sphere2.vel - capsule->vel;


  return false;
}


void PhysicsEngine::capsule_triangle_response(CapsuleCollider *capsule, Vertex v0, Vertex v1, Vertex v2, float dist, bool edge_collision, glm::vec3 dir)
{

}