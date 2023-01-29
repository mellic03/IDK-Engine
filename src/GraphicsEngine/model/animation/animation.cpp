#include "animation.h"


namespace Animation {



  Armature *Animation::getArmature()
  {
    return &this->_armature;
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






  Animation *AnimationController::getAnimation(std::string name)
  {
    return &this->_animations[name];
  }

}
