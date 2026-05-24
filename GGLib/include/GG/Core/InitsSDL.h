#pragma once

//#include <SDL3/SDL_main.h> // must be included wherever main() is TODO

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

namespace InitsSDL 
{
    bool initSDL();
    bool initTTF();
    bool initMixer();

    bool initAll();

    void quitSDL();
}
