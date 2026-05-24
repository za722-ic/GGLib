#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <vector>

#include "GG/MoreMath/MoreMath.h"

class AudioManager
{
private:
    std::weak_ptr<std::unordered_map<std::string, MIX_Audio*>> audioPtrs;

    MIX_Mixer* mixer;

    std::vector<MIX_Track*> tracks;

    size_t currTrackIndex = 0;

public:
    void init();

    MIX_Mixer* getMixer() const;

    void setAudioPtrs(std::weak_ptr<std::unordered_map<std::string, MIX_Audio*>> newAudioPtrs);

    void playAudio(std::string fileName);
    
    void setVolume(float percentage);
};

