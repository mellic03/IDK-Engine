#include "audio.h"

bool AudioEngine::free_channels[AUDIOENGINE_NUM_CHANNELS] = { true };
glm::vec3 *AudioEngine::listener_pos = nullptr;


void AudioEngine::init(void)
{
  int audio_rate = 44100;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_buffers = 4096;

  if (Mix_OpenAudio(audio_rate, audio_format, AUDIOENGINE_NUM_CHANNELS, audio_buffers) != 0)
  {
    printf("Error initialising SDL2 audio\n");
  }

  for (size_t i=0; i<AUDIOENGINE_NUM_CHANNELS; i++)
    AudioEngine::free_channels[i] = true;
}


int AudioEngine::getChannel(void)
{
  for (size_t i=0; i<AUDIOENGINE_NUM_CHANNELS; i++)
  {
    if (AudioEngine::free_channels[i] == true)
      return i + 1;
  }
  return -1;
}


void AudioEngine::freeChannel(int channel)
{
  AudioEngine::free_channels[channel - 1] = true;
}


void AudioEngine::reserveChannel(int channel)
{
  AudioEngine::free_channels[channel - 1] = false;
}
