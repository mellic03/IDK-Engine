#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL2/SDL_mixer.h>

#include "../include/glm/glm.hpp"


#define AUDIOENGINE_NUM_CHANNELS 8



namespace AudioEngine
{
  extern bool free_channels[AUDIOENGINE_NUM_CHANNELS]; 
  extern glm::vec3 *listener_pos;

  void init(void);

  int getChannel(void);
  void freeChannel(int channel);
  void reserveChannel(int channel);

};


