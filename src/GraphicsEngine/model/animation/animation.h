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

    std::string _name = "";
    float _length = 0.0f;
    float _current_time = 0.0f;



  public:
    Animation()  { };

    void setName(std::string name);
    std::string getName();

    void  setLength(float length);
    float getLength(void);


    Armature *getArmature();

    float getTime();
    void advance(float deltaTime);

};


class Animation::AnimationController {

  private:

    int _num_animations = 0;
    int _default_animation = 0;

    std::map<std::string, Animation> _animations;
    std::map<int, Animation *>       _animations_by_index;
  
    std::string _active_animation = "";
    bool _override_animation_state = false;


  public:
  
    AnimationController()  { };

    void setActiveAnimation(std::string name);

    Animation *getAnimation();
    Animation *getAnimation(std::string name);

    std::map<std::string, Animation> *getAnimations();

    Animation *newAnimation(std::string name);

};




