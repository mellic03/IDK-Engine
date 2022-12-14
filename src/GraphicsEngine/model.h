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

#include "../transform.h"

// #include "../GameEngine/player.h"
// class Player;

enum ModelState { MSTATE_NOANIM_PLAYING, MSTATE_ANIM_PLAYING };

class Model {

  private:

    bool m_rotate_locally = false;
    bool m_model_mat_set_manually = false;
    
    glm::mat4 m_model_mat = glm::mat4(1.0f);
    glm::mat4 *m_parent_model_mat = nullptr;
    glm::mat4 m_temp_model_mat = glm::mat4(1.0f);
    

    std::string m_name;

    bool is_animated = false;
    bool is_environmental = false;
    bool is_NPC = false;
  
    Mesh m_staticmesh;
    Mesh m_collision_mesh;

    ModelState m_state = MSTATE_NOANIM_PLAYING;

    AnimationType m_active_animation = ANIM_REST;
    std::vector<Animation> animations;


    Transform *m_transform = nullptr;
    AnimationController *m_animation_controller = nullptr;

  public:

    Model()
    {
      for (int i=0; i<NUM_ANIMATION_TYPES; i++)
        this->animations.push_back(Animation());
    };

    // Member Access
    //--------------------------------------
    Mesh *getCollisionMesh(void) { return &this->m_collision_mesh; };

    bool isAnimated(void)      { return this->is_animated; };
    bool isEnvironmental(void) { return this->is_environmental; };
    bool isNPC(void)           { return this->is_NPC; };

    std::string getName(void)  { return this->m_name; };

    std::vector<Animation> getAnimations(void) { return this->animations; };

    void setModelMat(glm::mat4 mat) { this->m_model_mat = mat; this->m_model_mat_set_manually = true; };
    void setParentModelMat(glm::mat4 *mat) { this->m_parent_model_mat = mat; };
    glm::mat4 *getModelMat(void) { return &this->m_model_mat; };
    //--------------------------------------

    bool load(std::string filepath);

    void activeAnimation(AnimationType id);
    void playAnimation(Renderer *ren);

    // void collideWithPlayer(Player *player);

    void useTransform(Transform *transform)
    {
      this->m_transform = transform;
      this->m_staticmesh.useTransform(transform);
      for (auto &animation: this->animations)
        animation.useTransform(transform);
    };

    void useAnimController(AnimationController *animation_controller)
    {
      this->m_animation_controller = animation_controller;
    }

    void drawStaticMesh(Renderer *ren);
    void drawAnimatedMesh(Renderer *ren);
    void draw(Renderer *ren);
};


class InstancedModel {

};
