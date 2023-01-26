#pragma once


enum class RenderDebugFlag: unsigned char
{
  None                  = 0b00000000,
  DrawBoundingSpheres   = 0b00000001,
  DrawColliders         = 0b00000010,

};


struct RenderDebugData
{
  private:
    RenderDebugFlag _debug_flags = RenderDebugFlag::None;

  public:
    RenderDebugFlag *getDebugFlags();
    bool getDebugFlag(RenderDebugFlag flag);
    void setDebugFlag(RenderDebugFlag flag, bool value);
};
