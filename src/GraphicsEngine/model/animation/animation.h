#pragma once

#include <vector>
#include <map>

#include "armature.h"



namespace Animation {

  class Animation;
  class AnimationController;

};


class Animation::Animation {

  private:
    Armature _armature;
    float _length = 0.0f;
    float _current_time = 0.0f;

  public:
    Animation()  { };

    void  setLength(float length);
    float getLength(void);


    Armature *getArmature();

    float getTime();
    void advance(float deltaTime);

};


class Animation::AnimationController {

  private:
    std::map<std::string, Animation> _animations = std::map<std::string, Animation>();


  public:
  
    AnimationController()  { };

    Animation *getAnimation(std::string name);

};




