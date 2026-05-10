#include "AudioManager.h"

void AudioManager::init()
{
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

    const int numTracks = 8;
    for (size_t i = 0; i < numTracks; i++)
    {
        tracks.push_back(MIX_CreateTrack(mixer));
    }
}

MIX_Mixer* AudioManager::getMixer() const
{
    return mixer;
}

void AudioManager::setAudioPtrs(std::weak_ptr<std::unordered_map<std::string, MIX_Audio*>> newAudioPtrs)
{
    audioPtrs = newAudioPtrs;
}

void AudioManager::playAudio(std::string fileName)
{
    if (!audioPtrs.lock()->contains(fileName))
    {
        std::cout << "ERROR: Failed to play audio (couldn't find file): " << fileName << std::endl;
        return;
    }

    MIX_Audio *audio = (*(audioPtrs.lock()))[fileName];

    if (!MIX_SetTrackAudio(tracks.at(currTrackIndex), audio))
    {
		std::cout << "ERROR: Failed to set track audio at track index " << currTrackIndex << std::endl << SDL_GetError() << std::endl;
    }

    if (!MIX_PlayTrack(tracks.at(currTrackIndex), 0))
	{
		std::cout << "ERROR: Failed to play track at track index " << currTrackIndex << std::endl << SDL_GetError() << std::endl;
	}

    currTrackIndex = (currTrackIndex + 1) % tracks.size(); // round-robin selection of which track we use. TODO is this a good way of doing it?
}

void AudioManager::setVolume(float percentage)
{
    /*
       clamp percentage from 0 to 1.5

        1.0 = normal volume
       >1.0 = amplification (we clamp to 1.5 to keep things safe)
       <1.0 = decrease volume
       <0 is illegal
    */
    percentage = MoreMath::clamp(percentage, 0.0f, 1.5f);

    // update the volume for every track
    for (auto track : tracks)
    {
        if (!MIX_SetTrackGain(track, percentage))
        {
            std::cout << "ERROR: Failed to set track gain" << std::endl << SDL_GetError() << std::endl;
        }
    }
}