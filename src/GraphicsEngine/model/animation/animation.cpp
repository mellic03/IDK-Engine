#include "animation.h"


namespace Animation {



  Armature *Animation::getArmature()
  {
    return &this->_armature;
  }

  void Animation::setName(std::string name)
  {
    this->_name = name;
  }

  std::string Animation::getName()
  {
    return this->_name;
  }

  void Animation::setLength(float length)
  {
    this->_length = length;
  }

  float Animation::getLength(void)
  {
    return this->_length;
  }


  float Animation::getTime()
  {
    return this->_current_time;
  }  

  void Animation::advance(float deltaTime)
  {
    if (this->_current_time + deltaTime <= this->_length)
      this->_current_time += deltaTime;

    else
      this->_current_time = deltaTime;
  }



  void AnimationController::setActiveAnimation(std::string name)
  {
    this->_active_animation = name;
  }



  Animation *AnimationController::getAnimation()
  {
    return &this->_animations[this->_active_animation];
  }

  Animation *AnimationController::getAnimation(std::string name)
  {
    return &this->_animations[name];
  }

  std::map<std::string, Animation> *AnimationController::getAnimations()
  {
    return &this->_animations;
  }


  Animation *AnimationController::newAnimation(std::string name)
  {
    this->_animations[name].setName(name);
    this->_active_animation = name;
  
    return &this->_animations[name];
  }
}
