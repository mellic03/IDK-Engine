#include <stdio.h>

#include "animation.h"


bool Animation::loadKeyframes(std::string filepath, std::string name, int num_keyframes)
{
  for (int i=0; i<num_keyframes; i++)
  {
    this->keyframes.push_back(Mesh());
    this->keyframes[this->keyframes.size()-1].load(filepath.c_str(), name + std::to_string(i+1));
  }

  for (int i=0; i<num_keyframes; i++)
  {
    for (int j=0; j<this->keyframes[i].num_vertices; j++)
      this->keyframes[i].vertices[j].anim_next_pos = this->keyframes[(i+1)%num_keyframes].vertices[j].position;

    this->keyframes[i].setBufferData();
  }

  printf("num_keyframes: %d\n", num_keyframes);

  return true;
}


void Animation::setPos(glm::vec3 *position)
{
  // for (int i=0; i<this->keyframes.size(); i++)
  //   this->keyframes[i].setPos(position);
}

void Animation::setRot(glm::vec3 *rotation)
{
//   for (int i=0; i<this->keyframes.size(); i++)
//     this->keyframes[i].setRot(rotation);
}

void Animation::setModelMat(glm::mat4 *mat)
{
  for (auto &keyframe: this->keyframes)
    keyframe.setModelMat(mat);
}


void Animation::nextFrame(void)
{
  this->m_active_keyframe = (this->m_active_keyframe + 1) % this->keyframes.size();
}


void Animation::play(Renderer *ren)
{
  float lerp_value = (float)this->m_anim_framecount / (float)this->m_anim_framelength;
  ren->active_shader->setFloat("lerp_value", lerp_value);

  this->keyframes[this->m_active_keyframe].draw(ren);

  if (this->m_anim_framecount >= this->m_anim_framelength)
  {
    this->m_anim_framecount = 0;
    this->nextFrame();
  }

  this->m_anim_framecount += 1;
}


