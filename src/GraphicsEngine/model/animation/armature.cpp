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


glm::mat4 Animation::Joint::getAnimationMatrix(float time)
{
  float closest_index = -1;
  float closest_value = INFINITY;

  for (size_t i=0; i<this->keyframe_times.size(); i++)
  {
    float value = fabs(time - this->keyframe_times[i]);
    if (value < closest_value)
    {
      closest_value = value;
      closest_index = i;
    }
  }

  return (closest_index == -1) ? this->localTransform : this->keyframe_matrices[closest_index];
}


static void recurse_computePose(Animation::Joint *joint, glm::mat4 parent_finalBoneTransform, float keyframe_time)
{
  joint->finalBoneTransform = parent_finalBoneTransform * joint->getAnimationMatrix(keyframe_time);

  for (Animation::Joint *child: joint->children)
    recurse_computePose(child, joint->finalBoneTransform, keyframe_time);

  joint->finalBoneTransform = joint->finalBoneTransform * joint->inverseBindTransform;
}


void Animation::Armature::computePose(float keyframe_time)
{
  recurse_computePose(this->root, glm::mat4(1.0f), keyframe_time);
}