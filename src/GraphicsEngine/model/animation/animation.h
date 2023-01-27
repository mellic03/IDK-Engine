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

  public:
    Animation()  { };

    Armature *getArmature();

};


class Animation::AnimationController {

  private:
    std::map<std::string, Animation> _animations = std::map<std::string, Animation>();


  public:
  
    AnimationController()  { };

    void setAnimation(std::string name, Animation animation);
    Animation *getAnimation(std::string name);

};




