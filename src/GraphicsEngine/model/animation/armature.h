#pragma once

#include <list>
#include <vector>
#include "../../transform/transform.h"


namespace Animation {

  constexpr int MAX_BONES = 64;

  struct Joint;
  class Armature;

};


struct Animation::Joint {

  bool is_animated = false;


  std::string _id_str = "";
  std::string _name_str = "";
  std::string _type_str = "";

  int _id = -1;

  glm::mat4 transform = glm::mat4(1.0f);
  glm::mat4 localTransform = glm::mat4(1.0f);
  glm::mat4 inverseBindTransform = glm::mat4(1.0f);

  glm::mat4 finalTransform = glm::mat4(1.0f);


  Joint *parent = nullptr;
  std::list<Joint *> children;

  std::vector<float> keyframe_times;
  std::vector<glm::mat4> keyframe_matrices;
  std::vector<glm::mat4> keyframe_finalBoneTransforms;

  Joint()  { };

  Joint(std::string id, std::string name, std::string type, glm::mat4 t);

  glm::mat4 getAnimationMatrix(float time);
  glm::mat4 getBoneTransformMatrix(float time);

  int indexOfKeyframeTime(float time);
};


class Animation::Armature {

  private:

    void _computeLocalTransforms(Animation::Joint *joint, glm::mat4 parent_localTransform);
    void _computePose(Animation::Joint *joint, glm::mat4 parent_finalBoneTransform, float keyframe_time);



  public:

    glm::mat4 globalInverseTransform = glm::mat4(1.0f);

    std::string name = "";
    Joint *root = nullptr;
    std::vector<Joint *> joints;


    Armature() { };

    Joint *find(std::string name_str);
    Joint *find(int id);

    void computeLocalTransforms();
    void computeFinalTransforms();
    void computePose(float keyframe_time);
};

