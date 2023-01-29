#include "armature.h"

#include "../../transform/transformutil.h"


Animation::Joint::Joint(std::string id, std::string name, std::string type, glm::mat4 t)
{
  std::size_t pos = name.find(".");
  if (pos != std::string::npos)
    name.replace(pos, 1, "_");
  
  this->_id_str   = id;
  this->_name_str = name;
  this->_type_str = type;
  this->transform = t;
};


int Animation::Joint::indexOfKeyframeTime(float time)
{
  int max_index = static_cast<int>(this->keyframe_times.size()) - 1;

  if (max_index == -1)
    return -1;

  for (int i=max_index; i>=0; i--)
  {
    if (time >= this->keyframe_times[i])
      return i;
  }

  if (time < this->keyframe_times[0])
    return max_index;


  printf("[Animation::Joint::indexOfKeyframeTime()] Should not be here!!\n");
  return -1;
}


glm::mat4 Animation::Joint::getAnimationMatrix(float time)
{
  int i = this->indexOfKeyframeTime(time);

  return (i == -1) ? this->localTransform : this->keyframe_matrices[i];
}


glm::mat4 Animation::Joint::getBoneTransformMatrix(float time)
{
  int index1 = this->indexOfKeyframeTime(time);

  if (index1 == -1)
  {
    if (this->parent != nullptr)
      return this->parent->getBoneTransformMatrix(time);
  
    else
      return glm::mat4(1.0f);
  }

  int index2 = (index1 + 1) % this->keyframe_finalBoneTransforms.size();

  float start;
  float stop;
  float alpha;

  if (index2 == 0)
    start = 0;
 
  else
    start = this->keyframe_times[index1];
 
  stop  = this->keyframe_times[index2];
  alpha = (time - start) / (stop - start);


  glm::mat4 mat1 = this->keyframe_finalBoneTransforms[index1];
  glm::mat4 mat2 = this->keyframe_finalBoneTransforms[index2];

  return TransformUtil::lerpMatrix(mat1, mat2, alpha);
}






void Animation::Armature::_computeLocalTransforms(Animation::Joint *joint, glm::mat4 parent_localTransform)
{
  joint->localTransform = parent_localTransform * joint->transform;

  for (Animation::Joint *child: joint->children)
    this->_computeLocalTransforms(child, joint->localTransform);
}


void Animation::Armature::computeLocalTransforms()
{
  this->_computeLocalTransforms(this->root, glm::mat4(1.0f));
}


void Animation::Armature::computeFinalTransforms()
{
  for (Animation::Joint *joint: this->joints)
  {
    if (joint->is_animated)
      joint->finalTransform = joint->inverseBindTransform * joint->localTransform * this->globalInverseTransform;
  
    else
      joint->finalTransform = joint->inverseBindTransform * this->globalInverseTransform;
  }
}




void Animation::Armature::_computePose(Animation::Joint *joint, glm::mat4 parent_animatedTransform, float keyframe_time)
{
  glm::mat4 animatedTransform = parent_animatedTransform * joint->localTransform * joint->getAnimationMatrix(keyframe_time);

  for (Animation::Joint *child: joint->children)
    this->_computePose(child, animatedTransform, keyframe_time);

  animatedTransform = animatedTransform * joint->inverseBindTransform;
  

  int index = joint->indexOfKeyframeTime(keyframe_time);
  if (index != -1)
    joint->keyframe_finalBoneTransforms[index] = animatedTransform;
}


void Animation::Armature::computePose(float keyframe_time)
{
  this->_computePose(this->root, this->root->finalTransform, keyframe_time);
}




Animation::Joint *Animation::Armature::find(std::string name_str)
{
  for (Joint *joint: this->joints)
    if (joint->_name_str == name_str || joint->_id_str == name_str)
      return joint;
  
  return nullptr;
}


Animation::Joint *Animation::Armature::find(int id)
{
  for (Joint *joint: this->joints)
    if (joint->_id == id)
      return joint;
  
  return nullptr;
}

