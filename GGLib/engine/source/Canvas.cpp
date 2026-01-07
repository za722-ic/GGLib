#include "Canvas.h"

// ------ setters ------

void Canvas::setRenderer(SDL_Renderer* newRenderer)
{
    renderer = newRenderer;
}

void Canvas::setAlignment(Alignment newAlignment)
{
    alignment = newAlignment;
}

void Canvas::setOrigin(int x, int y)
{
    originX = x;
    originY = y;
}

void Canvas::setOriginToCenter()
{
    setOrigin(getWidth() / 2, getHeight() / 2);
}

void Canvas::resetOrigin()
{
    setOrigin(0, 0);
}

void Canvas::setFont(TTF_Font* newFont)
{
    font = newFont;
}

void Canvas::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    setColor(r, g, b, 255);
}
void Canvas::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color c{ r, g, b, a };
    color = c;
}

void Canvas::setColor(Uint8 c)
{
    setColor(c, c, c, 255);
}

void Canvas::setColor(Uint8 c, Uint8 a)
{
    setColor(c, c, c, a);
}

void Canvas::setColor(SDL_Color c)
{
    color = c;
}


// ------ getters ------

int Canvas::getWidth()
{
    int w;
    SDL_RenderGetLogicalSize(renderer, &w, NULL);
    return w;
}

int Canvas::getHeight()
{
    int h;
    SDL_RenderGetLogicalSize(renderer, NULL, &h);
    return h;
}


// ------ draw funtions ------

void Canvas::clear()
{
    setSDLColorToCanvasColor();
    SDL_RenderClear(renderer);
}

void Canvas::present()
{
    SDL_RenderPresent(renderer);
}

void Canvas::drawRect(int x, int y, int w, int h)
{
    setSDLColorToCanvasColor();
    applyCanvasOrigin(x, y);
    applyCanvasAlignment(x, y, w, h);

	SDL_Rect rect{ x, y, w, h };
	SDL_RenderDrawRect(renderer, &rect);
}

void Canvas::drawRect(const SDL_Rect& rect)
{
    drawRect(rect.x, rect.y, rect.w, rect.h);
}

void Canvas::fillRect(int x, int y, int w, int h)
{
    setSDLColorToCanvasColor();
    applyCanvasOrigin(x, y);
    applyCanvasAlignment(x, y, w, h);
    
    SDL_Rect rect{ x, y, w, h };
	SDL_RenderFillRect(renderer, &rect);
}

void Canvas::fillRect(const SDL_Rect& rect)
{
    fillRect(rect.x, rect.y, rect.w, rect.h); // we have to create a new rect anyway if we want to pass the rect using const reference
}

void Canvas::drawLine(int x1, int y1, int x2, int y2)
{
    setSDLColorToCanvasColor();
    applyCanvasOrigin(x1, y1);
    applyCanvasOrigin(x2, y2);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Canvas::drawString(std::string text, int x, int y)
{
    int w, h;
    getTextDimensions(text, &w, &h);
    drawStringToDimensions(text, x, y, w, h);
}

void Canvas::drawStringToWidth(std::string text, int x, int y, int desiredWidth)
{
    // we have only been given a width and no height. we calculate a height in accordance with the aspect ratio given by getTextDimensions
    int w, h;
    getTextDimensions(text, &w, &h);
    h = (int)(desiredWidth * (float)h / w);

    drawStringToDimensions(text, x, y, desiredWidth, h);
}

void Canvas::drawStringToHeight(std::string text, int x, int y, int desiredHeight)
{
    // we have only been given a height and no width. we calculate a width in accordance with the aspect ratio given by getTextDimensions
    int w, h;
    getTextDimensions(text, &w, &h);
    w = (int)(desiredHeight * (float)w / h);

    drawStringToDimensions(text, x, y, w, desiredHeight);
}

void Canvas::drawStringToDimensions(std::string text, int x, int y, int w, int h)
{
    // if string is empty, we have nothing to do!
    if (text.size() == 0) return;

    // render text to texture
    SDL_Surface* textSurface = NULL;
    textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // error checking
    if (textSurface == NULL || textTexture == NULL)
    {
        std::cout << "Error in RenderText: (textSurface == NULL || textTexture == NULL)" << std::endl << SDL_GetError() << std::endl;
        return;
    }

    // determine offsets based on alignment option and canvas origin
    applyCanvasOrigin(x, y);
    applyCanvasAlignment(x, y, w, h);

    // render texture to canvas
    SDL_Rect textRect = { x, y, w, h };
    if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) < 0)
    {
        std::cout << "Error in RenderText: SDL_RenderCopy failed" << std::endl << SDL_GetError() << std::endl;
        return;
    }

    // free surface/texture used to render text
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void Canvas::drawImage(SDL_Texture* image, int x, int y, int w, int h)
{
    applyCanvasOrigin(x, y);
    applyCanvasAlignment(x, y, w, h);
    SDL_Rect rect{ x, y, w, h };
    SDL_RenderCopy(renderer, image, NULL, &rect);
}

void Canvas::drawImage(SDL_Texture* image, int x, int y)
{
    int w, h;
    SDL_QueryTexture(image, NULL, NULL, &w, &h);
    drawImage(image, x, y, w, h);
}

void Canvas::drawImageRegion(SDL_Texture* image, const SDL_Rect& region, int x, int y)
{
    drawImageRegion(image, region, x, y, region.w, region.h);
}

void Canvas::drawImageRegion(SDL_Texture* image, const SDL_Rect& region, int x, int y, int w, int h)
{
    applyCanvasOrigin(x, y);
    applyCanvasAlignment(x, y, w, h);
    SDL_Rect rect{ x, y, w, h };
    SDL_RenderCopy(renderer, image, &region, &rect);
}

void Canvas::drawImage_StrechToFillCanvas(SDL_Texture *image)
{
    SDL_RenderCopy(renderer, image, NULL, NULL);
}

// ------ helper methods ------

void Canvas::setSDLColorToCanvasColor()
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Canvas::applyCanvasOrigin(int& x, int& y)
{
    x += originX;
    y += originY;
}

void Canvas::applyCanvasAlignment(int &x, int &y, int w, int h)
{
    // resolve horizontal and vertical alignments
    HorizontalAlignment horizontalAlignment = getHorizontaAlignment(alignment);
    VerticalAlignment verticalAlignment = getVerticalAlignment(alignment);

    // determine horizontal offset based on horizontalAlignment option
    int offsetX;
    switch (horizontalAlignment)
    {
    case HorizontalAlignment::LEFT:   offsetX = 0;      break;
    case HorizontalAlignment::CENTER: offsetX = -w / 2; break;
    case HorizontalAlignment::RIGHT:  offsetX = -w;     break;
    default:     throw std::runtime_error("Unknown Alignment type");
    }

    // determine vertical offset based on verticalAlignment option
    int offsetY;
    switch (verticalAlignment)
    {
    case VerticalAlignment::TOP:    offsetY = 0;      break;
    case VerticalAlignment::CENTER: offsetY = -h / 2; break;
    case VerticalAlignment::BOTTOM: offsetY = -h;     break;
    default:     throw std::runtime_error("Unknown Alignment type");
    }

    // apply offsets to x, y
    x += offsetX;
    y += offsetY;
}
Canvas::HorizontalAlignment Canvas::getHorizontaAlignment(Alignment alignment)
{
    switch (alignment)
    {
    case Canvas::Alignment::TOP_LEFT:
    case Canvas::Alignment::CENTER_LEFT:
    case Canvas::Alignment::BOTTOM_LEFT:
        return HorizontalAlignment::LEFT;
    case Canvas::Alignment::TOP_CENTER:
    case Canvas::Alignment::CENTER_CENTER:
    case Canvas::Alignment::BOTTOM_CENTER:
        return HorizontalAlignment::CENTER;
    case Canvas::Alignment::TOP_RIGHT:
    case Canvas::Alignment::CENTER_RIGHT:
    case Canvas::Alignment::BOTTOM_RIGHT:
        return HorizontalAlignment::RIGHT;
    }

    throw std::runtime_error("Unknown Alignment type");
}
Canvas::VerticalAlignment Canvas::getVerticalAlignment(Alignment alignment)
{
    switch (alignment)
    {
    case Canvas::Alignment::TOP_LEFT:
    case Canvas::Alignment::TOP_CENTER:
    case Canvas::Alignment::TOP_RIGHT:
        return VerticalAlignment::TOP;
    case Canvas::Alignment::CENTER_LEFT:
    case Canvas::Alignment::CENTER_CENTER:
    case Canvas::Alignment::CENTER_RIGHT:
        return VerticalAlignment::CENTER;
    case Canvas::Alignment::BOTTOM_LEFT:
    case Canvas::Alignment::BOTTOM_CENTER:
    case Canvas::Alignment::BOTTOM_RIGHT:
        return VerticalAlignment::BOTTOM;
    }

    throw std::runtime_error("Unknown Alignment type");
}

void Canvas::getTextDimensions(std::string text, int* w, int* h) // FYI: upon experimentation, passing w or h as NULL does NOT cause an error!
{
    if (TTF_SizeText(font, text.c_str(), w, h) < 0)
    {
        std::cout << "TTF_SizeText() failed in TextRendering::GetTextDimensions(std::string text, int *w, int *h)" << std::endl;
        std::cout << SDL_GetError() << std::endl;
    }
}