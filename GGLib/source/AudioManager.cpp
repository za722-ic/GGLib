#include "AudioManager.h"

void AudioManager::setAudioPtrs(std::weak_ptr<std::unordered_map<std::string, Mix_Chunk*>> newAudioPtrs)
{
    audioPtrs = newAudioPtrs;
}

void AudioManager::playAudio(std::string fileName)
{
    if (!audioPtrs.lock()->contains(fileName))
    {
        std::cout << "ERROR: Failed to play audio " << fileName << std::endl;
        return;
    }

    Mix_PlayChannel(-1, (*(audioPtrs.lock()))[fileName], 0);
}

void AudioManager::setVolume(float percentage)
{
    // clamp percentage from 0...1
    percentage = MoreMath::clamp(percentage, 0.0f, 1.0f);

    // audio can go from 0...128
    percentage *= MIX_MAX_VOLUME;

    // update the volume for every sfx chunk
    auto it = audioPtrs.lock()->begin();
    while (it != audioPtrs.lock()->end())
    {
        Mix_VolumeChunk(it->second, percentage);
        it++;
    }
}