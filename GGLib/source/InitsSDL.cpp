#include "InitsSDL.h"

bool InitsSDL::initSDL()
{
    // Initialise SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL_Init failed" << std::endl << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool InitsSDL::initTTF()
{
    // Initialise SDL_TTF
    if (!TTF_Init())
    {
        std::cout << "TTF_Init failed" << std::endl << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool InitsSDL::initMixer()
{
    // Initialise SDL_mixer
    if (!MIX_Init())
    {
        std::cout << "MIX_Init failed" << std::endl << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}


bool InitsSDL::initAll()
{
    if (!initSDL()) return false;
    if (!initTTF()) return false;
    if (!initMixer()) return false;

    //if (!initImage()) return false; // unnecessary as of SDL3. leaving this here so that you know: https://github.com/libsdl-org/SDL_image/blob/main/docs/README-migration.md
    
    return true;
}

void InitsSDL::quitSDL()
{
    MIX_Quit();
    TTF_Quit();
    SDL_Quit();
}