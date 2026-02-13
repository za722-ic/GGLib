#include "Game.h"

Game::Game()
{
    // initialise SDL
    if (!InitsSDL::initAll()) 
        throw std::runtime_error("Failed to initialse SDL! See console output.");

    // create window and renderer
    std::optional<SDL_Renderer*> rendererOptional = gameWindow.init();
    if (!rendererOptional.has_value())
        throw std::runtime_error("Failed to create GameWindow! See console output.");

    SDL_Renderer* renderer = rendererOptional.value();

    // pass renderer to: canvas, asset manager, input manager
    canvas.init(renderer);
    assetManager.init(renderer);
    inputManager.init(renderer);

    // give audioManager pointer to all loaded audio files in assetManaegr
    audioManager.setAudioPtrs(assetManager.getAudioPtrs());

    // give uiManager pointer to input manager so that it can pass input events to ui stuff
    uiManager.init(&inputManager);
}

Game::~Game()
{
    InitsSDL::quitSDL();
}

void Game::gameLoop() 
{
    if (!onGameInit()) throw std::runtime_error("GameInit() failed!"); // can't call in constructor of Game as child classes of Game haven't had their constructor called yet 

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

        // If we didn't just detect the user quitting: update timers, run game loop and render
        if (isRunning)
        {
            updateTimers(dt);
            onGameLoop();
        }
    }

    onGameQuit();
}

double Game::deltaTime()
{
    return dt;
}

void Game::updateTimers(double deltaTime)
{
    for (GameTimer *timer : GameTimer::gameTimers)
    {
        timer->tickTimer(deltaTime);
    }
}
