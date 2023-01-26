#pragma once

#include <list>
#include "../../../transform.h"


namespace Animation {
  struct Joint;
  class Armature;
};


struct Animation::Joint {

  std::string _id_str = "";
  std::string _name_str = "";
  std::string _type_str = "";

  int _id = 0;
  glm::mat4 localBindTransform = glm::mat4(1.0f);
  glm::mat4 inverseBindTransform = glm::mat4(1.0f);

  Animation::Joint *parent = nullptr;
  std::list<Animation::Joint *> children;

  std::vector<float> keyframe_times;
  std::vector<glm::mat4> keyframe_matrices;

  Joint()  { };

  Joint(std::string id, std::string name, std::string type, glm::mat4 t)
  {
    std::size_t pos = name.find(".");
    if (pos != std::string::npos)
      name.replace(pos, 1, "_");
    
    this->_id_str   = id;
    this->_name_str = name;
    this->_type_str = type;
    this->localBindTransform = t;
  };


  glm::mat4 computeBindTransform()
  {
    Animation::Joint *joint = this->parent;
    glm::mat4 bind = this->localBindTransform;
    
    while (joint != nullptr)
    {
      bind = this->parent->computeBindTransform() * bind;
      joint = joint->parent;
    }

    return bind;  
  };

  glm::mat4 computeInvBindTransform()
  {
    return glm::inverse(this->computeBindTransform());
    // if (this->parent != nullptr)
    //   return this->parent->computeInvBindTransform() * this->inverseBindTransform;

    // else
    //   return this->inverseBindTransform;
  };



};


class Animation::Armature {

  public:
    std::string name = "";

    std::vector<Animation::Joint *> joints;

    Animation::Joint *root = nullptr;
    Armature() { };

    Animation::Joint *find(std::string name_str)
    {
      for (Animation::Joint *joint: this->joints)
        if (joint->_name_str == name_str || joint->_id_str == name_str)
          return joint;
      
      return nullptr;
    }

    Animation::Joint *find(int id)
    {
      for (Animation::Joint *joint: this->joints)
        if (joint->_id == id)
          return joint;
      
      return nullptr;
    }
};

