#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"




class ModelContainer {

  public:
    std::vector<Mesh *> models;
    ModelContainer(void) {};

    void add(Mesh *model);
    void draw(Renderer *ren);
    void collide(Player *player);
    void bindRenderer(Renderer *ren);
};


bool ray_intersect_triangle(  glm::vec3 ray_pos, glm::vec3 ray_dir,
                              glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                              glm::vec3 *intersect_point  );

float calculate_impulse(glm::vec3 vel, glm::vec3 face_normal, float mass);
