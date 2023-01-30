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
  glm::mat4 finalTransform_previous = glm::mat4(1.0f);


  Joint *parent = nullptr;
  std::list<Joint *> children;

  std::vector<float> keyframe_times;
  std::vector<glm::mat4> keyframe_matrices;
  std::vector<glm::mat4> keyframe_finalBoneTransforms;

  float alpha = 0.0f;

  Joint()  { };

  Joint(std::string id, std::string name, std::string type, glm::mat4 t);

  glm::mat4 getAnimationMatrix(float time);
  glm::mat4 getAnimationMatrix_blended(float time, Joint *joint2, float blendFactor);

  int indexOfKeyframeTime(float time);
};


class Animation::Armature {

  private:

    void _computeFinalTransforms(Joint *joint, glm::mat4 parent_localTransform, float keyframe_time);
    void _computeFinalTransforms_blended(Joint *joint, glm::mat4 parent_localTransform, float keyframe_time, float alpha);
    void _computePose(Joint *joint, glm::mat4 parent_finalBoneTransform, float keyframe_time);



  public:

    glm::mat4 globalInverseTransform = glm::mat4(1.0f);

    std::string name = "";
    Joint *root = nullptr;
    std::vector<Joint *> joints;


    Armature() { };

    Joint *find(std::string name_str);
    Joint *find(int id);

    void computePose(float keyframe_time);
    void computePose_blended(float keyframe_time, Armature *armature2, float alpha);

    void balanceKeyframes();

};

