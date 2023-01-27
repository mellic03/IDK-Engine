#include "armature.h"



Animation::Joint::Joint(std::string id, std::string name, std::string type, glm::mat4 t)
{
  std::size_t pos = name.find(".");
  if (pos != std::string::npos)
    name.replace(pos, 1, "_");
  
  this->_id_str   = id;
  this->_name_str = name;
  this->_type_str = type;
  this->localTransform = t;
};



static void recurse_computePose(Animation::Joint *joint, glm::mat4 parent_transform, int keyframe)
{
  glm::mat4 animationMatrix = glm::mat4(1.0f);

  if (joint->keyframe_matrices.size() > static_cast<size_t>(keyframe))
  {
    animationMatrix = joint->keyframe_matrices[keyframe];
  }
  

  joint->finalBoneTransform = parent_transform * joint->localTransform * animationMatrix;

  for (Animation::Joint *child: joint->children)
    recurse_computePose(child, joint->finalBoneTransform, keyframe);

  joint->finalBoneTransform = joint->finalBoneTransform * joint->inverseBindTransform;
  
}


void Animation::Armature::computePose(int keyframe)
{
  recurse_computePose(this->root, glm::inverse(this->root->inverseBindTransform), keyframe);
}