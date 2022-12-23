#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"


namespace PhysicsEngine {

  bool ray_intersect_triangle(  glm::vec3 ray_pos, glm::vec3 ray_dir,
                              glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                              glm::vec3 *intersect_point  );

  float calculate_impulse(glm::vec3 vel, glm::vec3 face_normal, float mass);
  
  glm::vec3 triangle_normal(glm::vec3 A, glm::vec3 B, glm::vec3 C);
  bool line_intersects_sphere(glm::vec3 A, glm::vec3 B, glm::vec3 P, float radius, glm::vec3 *dir);
  bool sphere_triangle_detect(GameObject *obj, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir);
  void sphere_triangle_response(GameObject *obj1, Vertex v0, Vertex v1, Vertex v2, float dist, bool edge_collision, glm::vec3 dir);
};




