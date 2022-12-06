#pragma once

#include "mesh.h"
#include "renderer.h"

enum AnimationType { ANIM_REST };
#define NUM_ANIMATION_TYPES 1


class Animation {

  private:

    unsigned int m_anim_framecount = 0;
    unsigned int m_anim_framelength = 144;
    unsigned int m_active_keyframe = 0;
    std::vector<Mesh> keyframes;
  
    bool m_looping = false;
    void nextFrame(void);
  
  
  public:
    Animation() { };

    bool loadKeyframes(std::string filepath, std::string name, int num_keyframes);

    void looping(bool loop) { this->m_looping = loop; };

    void play(Renderer *ren);
};