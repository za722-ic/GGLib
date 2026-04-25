#include "InitsSDL.h"

bool InitsSDL::initSDL()
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL_Init failed" << std::endl << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool InitsSDL::initTTF()
{
    // Initialise SDL_TTF
    if (TTF_Init() == -1)
    {
        std::cout << "TTF_Init failed" << std::endl << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

bool InitsSDL::initMixer()
{
    // Initialise SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        std::cout << "Mix_OpenAudio failed" << std::endl << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

bool InitsSDL::initImage()
{
    // Initialise SDL_image
    int flags = IMG_INIT_PNG;
    if (IMG_Init(flags) != flags)
    {
        std::cout << "IMG_Init failed to initialse for PNG" << std::endl << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}

bool InitsSDL::initAll()
{
    if (!initSDL()) return false;
    if (!initTTF()) return false;
    if (!initMixer()) return false;
    if (!initImage()) return false;
    return true;
}

void InitsSDL::quitSDL()
{
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}