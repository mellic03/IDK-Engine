#pragma once

#include "audio.h"


namespace AudioEngine
{
  struct Sound
  {
    glm::vec3 *pos = nullptr;
    float range = 10.0f;

    int channel = -1;
    Mix_Chunk *chunk = nullptr;
    
    void load(std::string filepath);

    void setVolume(float volume);

    bool playing(void);
    void play(void);
    void stop(void);

  };
}

