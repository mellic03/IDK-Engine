#include "armature.h"

#include <algorithm>

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
  int index1 = this->indexOfKeyframeTime(time);
  if (index1 == -1)
  {
    return this->transform;
  }

  int index2 = (index1 + 1) % this->keyframe_matrices.size();

  float start;
  float stop;

  if (index2 == 0)
    start = 0;
 
  else
    start = this->keyframe_times[index1];
 
  stop  = this->keyframe_times[index2];
  this->alpha = (time - start) / (stop - start);

  glm::mat4 mat1 = this->keyframe_matrices[index1];
  glm::mat4 mat2 = this->keyframe_matrices[index2];

  return TransformUtil::lerpMatrix(mat1, mat2, alpha);
}


glm::mat4 Animation::Joint::getAnimationMatrix_blended(float time, Animation::Joint *joint2, float blendFactor)
{
  int index1 = this->indexOfKeyframeTime(time);
  if (index1 == -1)
  {
    return TransformUtil::lerpMatrix(this->transform, joint2->transform, blendFactor);
  }

  int index2 = (index1 + 1) % this->keyframe_matrices.size();

  float start;
  float stop;

  if (index2 == 0)
    start = 0;
 
  else
    start = this->keyframe_times[index1];
 
  stop  = this->keyframe_times[index2];
  this->alpha = (time - start) / (stop - start);

  glm::mat4 mat1 = this->keyframe_matrices[index1];
  glm::mat4 mat2 = this->keyframe_matrices[index2];


  glm::mat4 thisMat = TransformUtil::lerpMatrix(mat1, mat2, alpha);

  mat1 = joint2->keyframe_matrices[index1];
  mat2 = joint2->keyframe_matrices[index2];

  glm::mat4 thatMat = TransformUtil::lerpMatrix(mat1, mat2, alpha);


  return TransformUtil::lerpMatrix(thisMat, thatMat, blendFactor);
}




void Animation::Armature::_computeFinalTransforms(Animation::Joint *joint, glm::mat4 parent_localTransform, float keyframe_time)
{
  joint->localTransform = parent_localTransform * joint->getAnimationMatrix(keyframe_time);
  joint->finalTransform = this->globalInverseTransform * joint->localTransform * joint->inverseBindTransform;

  for (Animation::Joint *child: joint->children)
    this->_computeFinalTransforms(child, joint->localTransform, keyframe_time);
}


void Animation::Armature::computePose(float keyframe_time)
{
  this->globalInverseTransform = glm::inverse(this->root->transform);
  this->_computeFinalTransforms(this->root, glm::mat4(1.0f), keyframe_time);
}


void Animation::Armature::computePose_blended(float keyframe_time, Animation::Armature *armature2, float alpha)
{
  this->globalInverseTransform = glm::inverse(this->root->transform);


  this->root->localTransform = this->root->getAnimationMatrix_blended(keyframe_time, armature2->root, alpha);

  for (size_t i=1; i<this->joints.size(); i++)
  {
    
    Animation::Joint *joint2 = armature2->find(this->joints[i]->_name_str);

    if (joint2 == nullptr)
      continue;

    this->joints[i]->localTransform =
      this->joints[i]->parent->localTransform *
      this->joints[i]->getAnimationMatrix_blended(keyframe_time, joint2, alpha);
  
    this->joints[i]->finalTransform = this->globalInverseTransform * this->joints[i]->localTransform * this->joints[i]->inverseBindTransform;
  }

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


void Animation::Armature::balanceKeyframes()
{
  std::vector<float> keyframes;

  for (auto &joint: this->joints)
    for (auto &keyframe: joint->keyframe_times)
      keyframes.push_back(keyframe);

  std::sort(keyframes.begin(), keyframes.end());

  for (auto &f: keyframes)
  {
    printf("%f ", f);
  }
  printf("\n");


}