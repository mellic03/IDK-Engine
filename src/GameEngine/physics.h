#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"

enum class PhysicsState {

  NONE,
  GROUNDED,
  FALLING

};


namespace PhysicsEngine {

  extern float gravity;

  struct SphereCollider {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 vel = glm::vec3(0.0f);
    float radius = 1.0f;
    float height_offset = 0.0f;
  };

  struct CapsuleCollider {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 vel = glm::vec3(0.0f);
    float top = 0.5f;
    float bottom = 2.0f;
    float radius  = 0.5f;
  };

  bool ray_intersects_plane(glm::vec3 ray_origin, glm::vec3 ray_dir, glm::vec3 plane_normal, glm::vec3 plane_pos);

  bool ray_intersects_triangle( glm::vec3 ray_pos, glm::vec3 ray_dir,
                                glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                                glm::vec3 *intersect_point  );

  float calculate_impulse(glm::vec3 vel, glm::vec3 face_normal, float mass);
  
  glm::vec3 triangle_normal(glm::vec3 A, glm::vec3 B, glm::vec3 C);
  bool line_intersects_sphere(glm::vec3 A, glm::vec3 B, glm::vec3 P, float radius, glm::vec3 *dir);
 
  bool sphere_triangle_detect(SphereCollider *spherecollider, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir);
  void sphere_triangle_response(SphereCollider *spherecollider, Vertex v0, Vertex v1, Vertex v2, float dist, bool edge_collision, glm::vec3 dir);

  bool capsule_triangle_detect(CapsuleCollider *capsule, Vertex v0, Vertex v1, Vertex v2, float *plane_dist, bool *edge_collision, glm::vec3 *dir);

};
