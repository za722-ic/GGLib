#pragma once

#include <SDL_mixer.h>

#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>

#include "MoreMath.h"

class AudioManager
{
private:
    std::weak_ptr<std::unordered_map<std::string, Mix_Chunk*>> audioPtrs;

public:

    void setAudioPtrs(std::weak_ptr<std::unordered_map<std::string, Mix_Chunk*>> newAudioPtrs);

    void playAudio(std::string fileName);
    
    void setVolume(float percentage);
};

