#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_TTF.h>

#include <iostream>

namespace InitsSDL 
{
    bool initSDL();
    bool initTTF();
    bool initMixer();
    bool initImage();

    bool initAll();

    void quitSDL();
}
