#pragma once

#include <list>
#include <vector>
#include "../../../transform.h"


namespace Animation {
  struct Joint;
  class Armature;
};


struct Animation::Joint {

  std::string _id_str = "";
  std::string _name_str = "";
  std::string _type_str = "";

  int _id = -1;
  glm::mat4 localTransform = glm::mat4(1.0f);
  glm::mat4 inverseBindTransform = glm::mat4(1.0f);
  glm::mat4 finalBoneTransform = glm::mat4(1.0f);

  Animation::Joint *parent = nullptr;
  std::list<Animation::Joint *> children;


  std::vector<float> keyframe_times;
  std::vector<glm::mat4> keyframe_matrices;

  Joint()  { };

  Joint(std::string id, std::string name, std::string type, glm::mat4 t);
};


class Animation::Armature {

  public:
    std::string name = "";

    std::vector<Animation::Joint *> joints;
    std::vector<Animation::Joint *> joints_sorted;

    Animation::Joint *root = nullptr;
    Armature() { };

    Animation::Joint *find(std::string name_str)
    {
      for (Animation::Joint *joint: this->joints)
        if (joint->_name_str == name_str || joint->_id_str == name_str)
          return joint;
      
      return nullptr;
    };

    Animation::Joint *find(int id)
    {
      for (Animation::Joint *joint: this->joints)
        if (joint->_id == id)
          return joint;
      
      return nullptr;
    };

    void sortJoints()
    {
      this->joints_sorted.clear();
      for (size_t i=0; i<this->joints.size(); i++)
      {
        Animation::Joint *joint = this->find(i);
        if (joint != nullptr)
          this->joints_sorted.push_back(joint);
      }
    };


    void computePose(int keyframe);
};

