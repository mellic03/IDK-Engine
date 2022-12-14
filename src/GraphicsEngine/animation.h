#pragma once

#include "mesh.h"
#include "renderer.h"

#include "../transform.h"

enum AnimationType { ANIM_REST };
#define NUM_ANIMATION_TYPES 1

class Animation {

  private:

    unsigned int m_anim_framecount = 0;
    unsigned int m_anim_framelength = 512;
    unsigned int m_active_keyframe = 0;
  
    bool m_looping = false;

    Transform *m_transform = nullptr;
  
  public:
    Animation() { };
    void nextFrame(void);

    std::vector<Mesh> keyframes;

    bool loadKeyframes(std::string filepath, std::string name, int num_keyframes);

    std::vector<Mesh> getKeyframes(void) { return this->keyframes; };

    void useTransform(Transform *transform)
    {
      for (auto &keyframe: this->keyframes)
        keyframe.useTransform(transform);
    };

    void looping(bool loop) { this->m_looping = loop; };

    void play(Renderer *ren);
};


struct AnimationController {

  std::vector<Animation> m_animations;
  AnimationType m_animation_type = ANIM_REST;

  int active_keyframe = 0, framecount = 0, framelength = 144;

  void assignAnimations(std::vector<Animation> animations) { this->m_animations = animations; };
  void useAnimation(AnimationType animation_type) { this->m_animation_type = animation_type; };


  void useTransform(Transform *transform)
  {
    for (auto &animation: this->m_animations)
      for (auto &keyframe: animation.keyframes)
        keyframe.useTransform(transform);
  };

  void playAnimation(Renderer *ren)
  {
    float lerp_value = (float)this->framecount / (float)this->framelength;
    ren->active_shader->setFloat("lerp_value", lerp_value);


    if (this->framecount >= this->framelength)
    {
      this->framecount = 0;
      this->active_keyframe = (this->active_keyframe + 1) % this->m_animations[this->m_animation_type].getKeyframes().size();
    }

    this->framecount += 1;
  
    this->m_animations[this->m_animation_type].getKeyframes()[this->active_keyframe].draw(ren);

  };
};
