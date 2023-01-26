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
  glm::mat4 transform = glm::mat4(1.0f);
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
    this->transform = t;
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

