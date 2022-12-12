#pragma once

#include "mesh.h"
#include "renderer.h"

enum AnimationType { ANIM_REST };
#define NUM_ANIMATION_TYPES 1


class Animation {

  private:
    std::vector<Mesh> keyframes;

    unsigned int m_anim_framecount = 0;
    unsigned int m_anim_framelength = 512;
    unsigned int m_active_keyframe = 0;
  
    bool m_looping = false;
    void nextFrame(void);

    // DEFAULT VALUES
    //----------------------------------------------
    //----------------------------------------------

  
  public:
    Animation() { };

    bool loadKeyframes(std::string filepath, std::string name, int num_keyframes);

    std::vector<Mesh> getKeyframes(void) { return this->keyframes; };

    void setModelMat(glm::mat4 *mat);

    void setPos(glm::vec3 *position);
    void setRot(glm::vec3 *rotation);

    void looping(bool loop) { this->m_looping = loop; };

    void play(Renderer *ren);
};