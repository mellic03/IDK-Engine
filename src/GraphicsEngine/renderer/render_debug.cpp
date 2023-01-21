#include "render_debug.h"


inline RenderDebugFlag operator|(RenderDebugFlag A, RenderDebugFlag B)
{
  return static_cast<RenderDebugFlag>(static_cast<unsigned char>(A) | static_cast<unsigned char>(B));
}


RenderDebugFlag *RenderDebugData::getDebugFlags()
{
  return &this->_debug_flags;
}


bool RenderDebugData::getDebugFlag(RenderDebugFlag flag)
{
  return static_cast<unsigned char>(this->_debug_flags) & static_cast<unsigned char>(flag);
}

void RenderDebugData::setDebugFlag(RenderDebugFlag flag, bool value)
{
  unsigned char mask = static_cast<unsigned char>(flag);
  unsigned char flags = static_cast<unsigned char>(this->_debug_flags);

  if (value)
    flags |= mask;
  else
    flags &= ~mask;
  
  this->_debug_flags = static_cast<RenderDebugFlag>(flags);
}