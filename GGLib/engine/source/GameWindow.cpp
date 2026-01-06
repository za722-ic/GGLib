#include <GameWindow.h>

std::optional<SDL_Renderer*> GameWindow::init()
{
    std::optional<SDL_Window*> windowOptional = createSDLWindow(640,  480, true, ""); // start off with fullscreen so that we can get the screen size; start off with 640x480 arbitrarily as it'll change with setRenderingResolution() later
    if (!windowOptional.has_value()) 
        return std::nullopt;
    else 
        window = windowOptional.value();

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "SDL_CreateRenderer failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    // Enable alpha blending
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
    {
        std::cout << "SDL_SetRenderDrawBlendMode failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    // Populate screenWidth, screenHeight (the dimensions of the display)
    int screenWidth, screenHeight;
    if (SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight) < 0)
    {
        std::cout << "SDL_GetRendererOutputSize failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    // Decide on a resolution to render at based on the screen resolution
    if (!setRenderingResolution(screenWidth, screenHeight)) return std::nullopt;

    return renderer;
}

void GameWindow::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    renderer = NULL;
    window = NULL;
}

void GameWindow::setTitle(std::string title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void GameWindow::setFullScreen(bool isFullScreen)
{
    Uint32 flags = isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    SDL_SetWindowFullscreen(window, flags);
}

void GameWindow::setResizable(bool isResizable)
{
    // TODO: this interacts poorly with setFullScreen
    SDL_SetWindowResizable(window, (SDL_bool)isResizable);
}

void GameWindow::setMouseHidden(bool isMouseHidden)
{
    
    if (SDL_ShowCursor(!isMouseHidden)  < 0)
    { 
        std::cout << "SDL_ShowCursor failed" << std::endl << SDL_GetError() << std::endl;
    }
}

bool GameWindow::getFullScreen()
{
    return SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

void GameWindow::toggleFullScreen()
{
    setFullScreen(!getFullScreen());
}



bool GameWindow::setRenderingResolution(int screenWidth, int screenHeight)
{
    // Decide on a logical resolution to render at
    // The decision process is as follows:
    // We want to use whatever the screen width/height is so USUALLY use that
    // We also want to use 16:9, so we decide a rendering resolution based on width and calculate a height according to a 16:9 aspect ratio. Then, if the screen resolution is not 16:9, letterboxing is introduced via SDL_RenderSetLogicalSize
    // If the screen resolution is too small/big, we'll keep rendering at the smallest/biggest viable resolution and then SDL_RenderSetLogicalSize will downscale/upscale

    const int MAX_WIDTH = 1920;
    const int MIN_WIDTH = 720;

    int renderWidth, renderHeight;

    if (screenWidth > 1920)
    {
        renderWidth = 1920;
        std::cout << "HIGH SCREEN RESOLUTION DETECTED - DEFAULTING TO 1920x1080. UPSCALING WILL TAKE PLACE. YOU MAY SEE RENDERING ARTIFACTS IF NOT USING A RESOLUTION THAT IS AN INTEGER MULTIPLE OF 1920x1080." << std::endl;
    }
    else if (screenWidth < 1280)
    {
        renderWidth = 1280;
        std::cout << "LOW SCREEN RESOLUTION DETECTED - DEFAULTING TO 1280x720. DOWNSCALING WILL TAKE PLACE. YOU MAY SEE RENDERING ARTIFACTS :(" << std::endl;
    }
    else
    {
        renderWidth = screenWidth;
    }

    renderHeight = renderWidth * 9.0f / 16.0f;

    if (SDL_RenderSetLogicalSize(renderer, renderWidth, renderHeight) < 0)
    {
        std::cout << "SDL_RenderSetLogicalSize failed" << std::endl << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        std::cout << "DETECTED SCREEN RESOLUTION: " << screenWidth << "x" << screenHeight << std::endl;
        std::cout << "USING LOGICAL RESOLUTION:   " << renderWidth << "x" << renderHeight << std::endl << std::endl;
    }

    return true;
}

std::optional<SDL_Window*> GameWindow::createSDLWindow(int width, int height, bool isFullScreen, std::string title)
{
    Uint32 flags = SDL_WINDOW_SHOWN; 
    if (isFullScreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

    if (window == NULL)
    {
        std::cout << "SDL_CreateWindow failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    return window;
}
