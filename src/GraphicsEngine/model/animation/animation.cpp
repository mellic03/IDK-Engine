#include "animation.h"


namespace Animation {



  Armature *Animation::getArmature()
  {
    return &this->_armature;
  }



  void AnimationController::setAnimation(std::string name, Animation animation)
  {
    this->_animations[name] = animation;
  }


  Animation *AnimationController::getAnimation(std::string name)
  {
    return &this->_animations[name];
  }

}
