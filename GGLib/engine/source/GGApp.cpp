#include "GGApp.h"

GGApp::GGApp()
{
    // initialise SDL
    if (!InitsSDL::initAll()) 
        throw std::runtime_error("Failed to initialse SDL! See console output.");

    // create window and renderer
    std::optional<SDL_Renderer*> rendererOptional = ggWindow.init();
    if (!rendererOptional.has_value())
        throw std::runtime_error("Failed to create application window! See console output.");

    SDL_Renderer* renderer = rendererOptional.value();

    // pass renderer to: canvas, asset manager, input manager
    canvas.init(renderer);
    assetManager.init(renderer);
    inputManager.init(renderer);

    // give audioManager pointer to all loaded audio files in assetManaegr
    audioManager.setAudioPtrs(assetManager.getAudioPtrs());
}

GGApp::~GGApp()
{
    InitsSDL::quitSDL();
}

void GGApp::mainLoop() 
{
    if (!onInit()) throw std::runtime_error("onInit() failed!"); // can't call in constructor of GGApp as child classes of GGApp haven't had their constructor called yet 

    isRunning = true;

    Uint64 currTime = SDL_GetPerformanceCounter();
    Uint64 prevTime = 0;
    dt = 0;  // deltaTime

    while (isRunning)
    {
        // Update deltaTime
        prevTime = currTime;
        currTime = SDL_GetPerformanceCounter();
        dt = 0.001 * (double)((currTime - prevTime) * 1000 / (double)SDL_GetPerformanceFrequency()); // (number of counts per frame)/(number of counts per second) = seconds per frame 

        // Handle events/user input
        if (!inputManager.pollEvents()) isRunning = false;

        // If we didn't just detect the user quitting: update timers, run user's main loop code and render
        if (isRunning)
        {
            updateTimers(dt);
            onLoop();
        }
    }

    onQuit();
}

double GGApp::deltaTime()
{
    return dt;
}

void GGApp::updateTimers(double deltaTime)
{
    for (Timer *timer : Timer::timers)
    {
        timer->tickTimer(deltaTime);
    }
}
