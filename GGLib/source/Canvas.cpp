#include "Canvas.h"

// TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually 

// ------ setters ------

void Canvas::init(SDL_Renderer* newRenderer)
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

// TODO: RenderGetLogicalSize is only valid if you actually set the logical size, which we no longer do as we just use the window size, so these return 0: https://wiki.libsdl.org/SDL2/SDL_RenderGetLogicalSize --> in any case, it is confusing for the user to have a separate window/canvas size (maybe just make it so that you can draw on the window, and just merge canvas into window?)
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

SDL_Color Canvas::getColor()
{
    return color;
}

SDL_Renderer* Canvas::getSDLRenderer()
{
    return renderer;
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

void Canvas::drawRect(int x, int y, int w, int h, int thickness)
{
    setSDLColorToCanvasColor();
    applyCanvasOrigin(x, y);
    applyCanvasAlignment(x, y, w, h);

    // use 4 rectangles for each of the 4 sides of the rect
	SDL_Rect top    { x,                 y, w,         thickness         };
    SDL_Rect left   { x,                 y, thickness, h                 };
    SDL_Rect right  { x + w - thickness, y, thickness, h                 };
    SDL_Rect bottom { x,                 y + h - thickness, w, thickness };

	SDL_RenderFillRect(renderer, &top);
	SDL_RenderFillRect(renderer, &left);
	SDL_RenderFillRect(renderer, &right);
	SDL_RenderFillRect(renderer, &bottom);
}

void Canvas::drawRect(const SDL_Rect& rect, int thickness)
{
    drawRect(rect.x, rect.y, rect.w, rect.h, thickness);
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

Vec2D deCasteljau_Cubic(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2, const Vec2D& p3, const float t)
{
    Vec2D B(0);

    B += p0 * 1.0f * std::pow(t, 0) * std::pow(1 - t, 3);
    B += p1 * 3.0f * std::pow(t, 1) * std::pow(1 - t, 2);
    B += p2 * 3.0f * std::pow(t, 2) * std::pow(1 - t, 1);
    B += p3 * 1.0f * std::pow(t, 3) * std::pow(1 - t, 0);

    return B;
}

Vec2D deCasteljau_Cubic_Derivative(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2, const Vec2D& p3, const float t)
{
    Vec2D dB(0);

    dB += p0 * (-3.0f) * std::pow(1 - t, 2);
    dB += p1 * (3.0f) * (std::pow(1 - t, 2) - 2 * t * (1 - t));
    dB += p2 * (3.0f) * (2 * t * (1 - t) - t * t);
    dB += p3 * (3.0f) * (t * t);

    dB.normalize();

    return dB;
}

void Canvas::drawCubicBezier(Vec2D p0, Vec2D p1, Vec2D p2, Vec2D p3, int thickness, int numSegments)
{
    std::vector<SDL_Vertex> vertices;

    // used to calculate vertices
    // we can't just evaluate the Bezier function B(t), as the resulting line would have thickeness=1
    // to support arbitrary thickness, we have to extrude the line out
    // this function calculates points on the extruded lines
    auto calcExtrudedPoints = [&](int segmentIndex)
    {
        // convert segment index to a parametric t-value for the Bezier function B
        float t =  (float)segmentIndex / numSegments;

        // evaluate the Bezier function B at t to get some 2d position on the curve --> B(t)
        Vec2D B = deCasteljau_Cubic(p0, p1, p2, p3, t);

        // evaluate the derivative at at --> dB(t)/dt
        // this gives the tangent direction of the curve B at location t along the curve
        Vec2D tangent = deCasteljau_Cubic_Derivative(p0, p1, p2, p3, t);

        // calculate the direction of the normals, which is the tangent rotated by 90deg
        Vec2D normal(-tangent.y, tangent.x);

        // calculate the extruded points
        // these points start at B, and extend in the direction of the normal
        Vec2D BL = B + normal * (thickness / 2);
        Vec2D BR = B - normal * (thickness / 2);

        vertices.push_back(BL.toSDLVertex(color));
        vertices.push_back(BR.toSDLVertex(color));
    };

    // calculate vertices
    for (int i = 0; i < numSegments; i++)
    {
        calcExtrudedPoints(i);
        calcExtrudedPoints(i + 1);
    }

    // calculate indices
    auto indices = getIndices_TriangleStrip(vertices.size());

    // render
    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());
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
    SDL_Texture* textTexture = drawStringToTexture(text);

    if (textTexture == nullptr) return;

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

    // free texture used to render text
    SDL_DestroyTexture(textTexture);
}

SDL_Texture* Canvas::drawStringToTexture(std::string text)
{
    // if string is empty, we have nothing to do!
    if (text.empty()) return nullptr;

    // render text to texture
    SDL_Surface* textSurface = NULL;
    textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color );// TODO: there are other quality levels, and a family of functions that are not "RenderUTF8" (though they are not recommended). Play with these (maybe make configurable), and also see if you can do something about ligatures looking weird (in particular "ti" appears bold). Read the docs: https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Blended 
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); 

    // error checking
    if (textSurface == NULL || textTexture == NULL)
    {
        std::cout << "Error in RenderText: (textSurface == NULL || textTexture == NULL)" << std::endl << SDL_GetError() << std::endl;
        return nullptr;
    }

    // free surface used to render text
    SDL_FreeSurface(textSurface);

    // return texture
    return textTexture;
}

void Canvas::getTextDimensions(std::string text, int* w, int* h) // FYI: upon experimentation, passing w or h as NULL does NOT cause an error!
{
    if (TTF_SizeText(font, text.c_str(), w, h) < 0)
    {
        std::cout << "TTF_SizeText() failed in TextRendering::GetTextDimensions(std::string text, int *w, int *h)" << std::endl;
        std::cout << SDL_GetError() << std::endl;
    }
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



std::vector<SDL_Vertex> Canvas::getRoundedRectVertices(int x, int y, int w, int h, int r, int trianglesPerCorner, SDL_Color color)
{
    std::vector<SDL_Vertex> verts;


    // centers of the quarter circles
    Vec2D cTopRight(x + w - r, y + r);
    Vec2D cTopLeft(x + r, y + r);
    Vec2D cBottomLeft (x + r, y + h - r);
    Vec2D cBottomRight(x + w - r, y + h - r);

    auto appendRoundedCornerVerts = [&](Vec2D center, float startingAngle)
    {
        for (int i = 0; i <= trianglesPerCorner; i++) // using <= because need n + 1 points on circle perimeter for n triangles
        {
            float theta = MoreMath::map((float)i / trianglesPerCorner, 0, 1, startingAngle, startingAngle + M_PI / 2);

            Vec2D vert(cosf(theta), sinf(theta));

            vert.y *= -1; // in math space, bigger y is higher. in screen space, biger y is lower
            vert *= r;
            vert += center;

            verts.push_back(vert.toSDLVertex(color));
        }
    };

    appendRoundedCornerVerts(cTopRight, 0.0f);
    appendRoundedCornerVerts(cTopLeft, M_PI / 2.0f);
    appendRoundedCornerVerts(cBottomLeft, M_PI);
    appendRoundedCornerVerts(cBottomRight, 3 * M_PI / 2.0f);

    return verts;
}

std::vector<SDL_Vertex> Canvas::interleaveVertices(const std::vector<SDL_Vertex>& v1, const std::vector<SDL_Vertex>& v2)
{
    // we expect both vertex lists to be of the same size
    // TODO: maybe just add the remaining vertices at the end e.g, 121212222
    assert(v1.size() == v2.size());

    std::vector<SDL_Vertex> result;

    for (int i = 0; i < v1.size(); i++) 
    {
        result.push_back(v1[i]);
        result.push_back(v2[i]);
    }

    return result;
}

std::vector<int> Canvas::getIndices_TriangleFan(int numVertices)
{
    std::vector<int> indices;

    int numTriangles = numVertices - 2; // true for any polygon

    for (int i = 0; i < numTriangles; i++)
    {
        indices.push_back(0); // triangle fan has one common vertex to all triangles
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    return indices;
}

std::vector<int> Canvas::getIndices_TriangleStrip(int numVertices)
{
    std::vector<int> indices;

    int numTriangles = numVertices - 2; // true for any polygon

    for (int i = 0; i < numTriangles; i++)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }


    return indices;
}

std::vector<int> Canvas::getIndices_TriangleStrip_Loop(int numVertices)
{
    std::vector<int> indices = getIndices_TriangleStrip(numVertices);

    // need to add two more triangles to close the triangle fan into a loop
    // which means connecting the last two vertices (at index numVertices - 1
    // and numVertices - 2) to the first two vertices (at index 0 and 1)
    indices.push_back(numVertices - 2);
    indices.push_back(numVertices - 1);
    indices.push_back(0);

    indices.push_back(numVertices - 1);
    indices.push_back(0);
    indices.push_back(1);

    return indices;
}

// TODO: this does not adhere to applyCanvasAlignment, applyCanvasOrigin
void Canvas::drawRoundedRect(int x, int y, int w, int h, int r, int thickness, SDL_Color innerColor, SDL_Color outerColor, const unsigned int trianglesPerCorner)
{
    int outerRadius = r;
    int innerRadius = outerRadius - thickness;

    std::vector<SDL_Vertex> vertsOuter = getRoundedRectVertices(x, y, w, h, outerRadius, trianglesPerCorner, outerColor);
    std::vector<SDL_Vertex> vertsInner = getRoundedRectVertices(x + thickness, y + thickness, w - 2*thickness, h - 2*thickness, innerRadius, trianglesPerCorner, innerColor);

    std::vector<SDL_Vertex> verts = interleaveVertices(vertsOuter, vertsInner);

    std::vector<int> indices = getIndices_TriangleStrip_Loop(verts.size());

    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), indices.data(), indices.size());
}

// TODO: this does not adhere to applyCanvasAlignment, applyCanvasOrigin
void Canvas::fillRoundedRect(int x, int y, int w, int h, int r, const unsigned int trianglesPerCorner)
{
    // // r cannot be greater than w or h, otherwise the quarter circles that get rendered are too big
    // // TODO: the case where r is greater than w or h makes this look weird --> how does CSS handle it?
    // if (r > w/2) r = w/2;
    // if (r > h/2) r = h/2;

    std::vector<SDL_Vertex> verts = getRoundedRectVertices(x, y, w, h, r, trianglesPerCorner, color);
    std::vector<int> indices = getIndices_TriangleFan(verts.size());
    
    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), indices.data(), indices.size());
}

std::vector<SDL_Vertex> Canvas::generateArcVertices(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, bool isChord)
{
    /*
        Starts at start angle
        Draws anti-clockwise until end angle
    */

    // special case: there is no arc to draw, so abort early
    if (endAngle == startAngle) return {};

    // radii
    int rHor = w / 2;
    int rVer = h / 2;
    
    // TAU
    const float TWO_PI = 2.0f * M_PI;

    // special case: there is no arc to draw, so abort early
    while (endAngle <= startAngle) endAngle += TWO_PI;
    while (std::abs(endAngle - startAngle) > TWO_PI) endAngle -= TWO_PI;

    // calculate angularDist
    const float angularDist = endAngle - startAngle;

    // calculate angularInterval between vertices
    const float angularInterval = angularDist / numTriangles;





    // vertices of the shape
    std::vector<SDL_Vertex> vertices;

    // center of shape
    Vec2D center(x + rHor, y + rVer);
    if (!isChord) vertices.push_back(center.toSDLVertex(color));
    
    // calculate vertices on the curved part of the arc's perimeter
    for (int i = 0; i <= numTriangles; i++)
    {
        float angle = startAngle + (i * angularInterval);

        Vec2D v(rHor * std::cos(angle), rVer * std::sin(angle));
        v.y *= -1; // in math space, y grows upwards. in screen space, y grows downwards
        v += center;

        vertices.push_back(v.toSDLVertex(color));
    }

    return vertices;
}

std::vector<SDL_Vertex> Canvas::generateArcNormals(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, int thickness)
{

    /*
        Starts at start angle
        Draws anti-clockwise until end angle
    */

    // special case: there is no arc to draw, so abort early
    if (endAngle == startAngle) return {};

    // radii
    int rHor = w / 2;
    int rVer = h / 2;

    // TAU
    const float TWO_PI = 2.0f * M_PI;

    // ensure endAngle > startAngle and angularDist is withing 
    while (endAngle <= startAngle) endAngle += TWO_PI;
    while (std::abs(endAngle - startAngle) > TWO_PI) endAngle -= TWO_PI;

    // calculate angularDist
    const float angularDist = endAngle - startAngle;

    // special case: there is no arc to draw, so abort early
    if (angularDist == 0.0f) return {};

    // calculate angularInterval between vertices
    const float angularInterval = angularDist / numTriangles;




    // vertices of the shape
    std::vector<SDL_Vertex> vertices;

    // center of shape
    Vec2D center(x + rHor, y + rVer);
    
    // calculate vertices on the curved part of the arc's perimeter
    for (int i = 0; i <= numTriangles; i++)
    {
        float angle = startAngle + (i * angularInterval);

        Vec2D v(rHor * std::cos(angle), rVer * std::sin(angle));
        v.y *= -1; // in math space, y grows upwards. in screen space, y grows downwards
        v += center;

        Vec2D n(rVer * std::cos(angle), -rHor * sin(angle));
        n.normalize();
        n *= thickness;
        
        Vec2D innerV = v - n;

        vertices.push_back(innerV.toSDLVertex(color));
    }

    return vertices;
}

void Canvas::drawArc(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, int thickness)
{
    auto outerVertices = generateArcVertices(x, y, w, h, startAngle, endAngle, numTriangles, true);
    auto innerVertices = generateArcNormals(x, y, w, h, startAngle, endAngle, numTriangles, thickness);
    
    auto interleavedVertices = interleaveVertices(outerVertices, innerVertices);
    auto indices = getIndices_TriangleStrip(interleavedVertices.size());

    SDL_RenderGeometry(renderer, nullptr, interleavedVertices.data(), interleavedVertices.size(), indices.data(), indices.size());
}

void Canvas::fillArc(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, bool isChord)
{
    auto vertices = generateArcVertices(x, y, w, h, startAngle, endAngle, numTriangles, isChord);
    auto indices = getIndices_TriangleFan(vertices.size());

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());
}


// TODO: this does not adhere to applyCanvasAlignment, applyCanvasOrigin
// TODO: x y here specify the center bby default, not the top-left --> how does p5.js handle this? top left of bounding box?
void Canvas::renderRegularPolygon(int x, int y, unsigned int numVertices, float rotAngle, float scale)
{
    const float INTERVAL = (2.0f * M_PI) / numVertices;

    std::vector<SDL_Vertex> verts;

    for (int i = 0; i < numVertices; i++)
    {
        float angle = rotAngle + INTERVAL * i;

        SDL_Vertex v1 =
        {
            SDL_FPoint{x + scale * cosf(angle), y + scale * sinf(angle)},
            color,
            SDL_FPoint { 0 }
        };

        angle = rotAngle + INTERVAL * (i + 1);

        SDL_Vertex v2 =
        {
            SDL_FPoint{x + scale * cosf(angle), y + scale * sinf(angle)},
            color,
            SDL_FPoint { 0 }
        };

        SDL_Vertex v3 =
        {
            SDL_FPoint{(float)x, (float)y},
            color,
            SDL_FPoint { 0 }
        };

        verts.push_back(v1);
        verts.push_back(v2);
        verts.push_back(v3);
    }

    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
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
