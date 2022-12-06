#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "mesh.h"
#include "animation.h"
#include "renderer.h"

#include "../GameEngine/navmesh.h"

enum ModelState { MSTATE_NOANIM_PLAYING, MSTATE_ANIM_PLAYING };


class Model {

  private:

    bool staticmesh = true;

    std::vector<Mesh> meshes;

    ModelState m_state = MSTATE_NOANIM_PLAYING;

    AnimationType m_active_animation = ANIM_REST;
    Animation animations[NUM_ANIMATION_TYPES];

  public:

    glm::vec3 *position;
    Mesh m_collision_mesh;

    Model() { };
    
    bool load(std::string filepath);

    void bindRenderer(Renderer *ren);

    void activeAnimation(AnimationType id);
    void playAnimation(Renderer *ren);

    void draw(Renderer *ren);
};

class TerrainModel: private Model {

};

class InstancedModel {

};
