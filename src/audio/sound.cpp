#include "sound.h"


void AudioEngine::Sound::load(std::string filepath)
{
  this->chunk = Mix_LoadWAV(filepath.c_str());
  if (this->chunk == nullptr)
    printf("Error opening WAV file: %s\n", filepath.c_str());

  this->setVolume(1.0f);
}


void AudioEngine::Sound::setVolume(float volume)
{
  volume = glm::clamp(volume, 0.0f, 1.0f);
  volume *= 128.0f;

  Mix_VolumeChunk(this->chunk, (int)volume);
}


bool AudioEngine::Sound::playing(void)
{
  return Mix_Playing(this->channel) > 0;
}


void AudioEngine::Sound::play(void)
{
  if (this->channel == -1)
  {
    int chnl = AudioEngine::getChannel();
    if (chnl == -1)
    {
      printf("No free channels\n");
      return;
    }
    AudioEngine::reserveChannel(chnl);
    this->channel = chnl;
  }


  if (this->pos != nullptr && AudioEngine::listener_pos != nullptr)
  {
    float d = glm::distance(*this->pos, *AudioEngine::listener_pos);
    float volume = 1.0f - (d / this->range);
    this->setVolume(volume);
  }

  if (this->playing())
    return;

  int loops = 0;

  if (Mix_PlayChannel(this->channel, this->chunk, loops) == -1)
  {
    printf("Error playing WAV file\n");
  }
}


void AudioEngine::Sound::stop(void)
{
  if (this->channel == -1)
    return;

  Mix_HaltChannel(this->channel);
  AudioEngine::freeChannel(this->channel);
  this->channel = -1;
}


