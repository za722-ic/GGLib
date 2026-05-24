#include "GG/Core/GGWindow.h"

std::optional<SDL_Renderer*> GGWindow::init()
{
    std::optional<SDL_Window*> windowOptional = createSDLWindow(640,  480, false, ""); // default to 640x480, not fullscreen, no title (user can manually override each of these)
    if (!windowOptional.has_value()) 
        return std::nullopt;
    else 
        window = windowOptional.value();

    // Create renderer
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL)
    {
        std::cout << "SDL_CreateRenderer failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    // Enable alpha blending
    if (!SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
    {
        std::cout << "SDL_SetRenderDrawBlendMode failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    // Populate screenWidth, screenHeight (the dimensions of the display)
    int screenWidth, screenHeight;
    if (!SDL_GetCurrentRenderOutputSize(renderer, &screenWidth, &screenHeight))
    {
        std::cout << "SDL_GetRendererOutputSize failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    return renderer;
}

void GGWindow::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    renderer = NULL;
    window = NULL;
}

void GGWindow::setTitle(std::string title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void GGWindow::setFullScreen(bool isFullScreen)
{
//    Uint32 flags = isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
//    SDL_SetWindowFullscreen(window, flags);
}

void GGWindow::setResizable(bool isResizable)
{
    // TODO: this interacts poorly with setFullScreen
    SDL_SetWindowResizable(window, isResizable);
}

void GGWindow::setMouseHidden(bool isMouseHidden)
{
    bool isSuccess;

    if (isMouseHidden) isSuccess = SDL_HideCursor();
    else               isSuccess = SDL_ShowCursor();

    if (!isSuccess) std::cout << "SDL_ShowCursor failed" << std::endl << SDL_GetError() << std::endl;
}

void GGWindow::setSize(unsigned int width, unsigned int height)
{
    SDL_SetWindowSize(window, width, height);
}

void GGWindow::setMinimumSize(unsigned int width, unsigned int height)
{
    SDL_SetWindowMinimumSize(window, width, height);
}

void GGWindow::setPos(int x, int y)
{
    SDL_SetWindowPosition(window, x, y);
}

void GGWindow::centerWindowPosition()
{
    setPos(
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED
    );
}

bool GGWindow::getFullScreen()
{
//    return SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
    return false;
}

unsigned int GGWindow::getWidth()
{
    int w;
    SDL_GetWindowSize(window, &w, nullptr);
    return w;
}

unsigned int GGWindow::getHeight()
{
    int h;
    SDL_GetWindowSize(window, nullptr, &h);
    return h;
}

void GGWindow::toggleFullScreen()
{
    setFullScreen(!getFullScreen());
}
	
SDL_Window* GGWindow::getSDLWindow()
{
    return window;
}

std::optional<SDL_Window*> GGWindow::createSDLWindow(unsigned int width, unsigned int height, bool isFullScreen, std::string title)
{
    Uint32 flags = 0; 
//    if (isFullScreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(title.c_str(), width, height, flags);

    if (window == NULL)
    {
        std::cout << "SDL_CreateWindow failed" << std::endl << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    return window;
}
