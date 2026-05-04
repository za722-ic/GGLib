#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Vec2D.h"
#include "MoreMath.h"

class Canvas
{
public:
	enum class HorizontalAlignment
	{
		LEFT,
		CENTER,
		RIGHT
	};

	enum class VerticalAlignment
	{
		TOP,
		CENTER,
		BOTTOM
	};

	enum class Alignment
	{
		TOP_LEFT,
		CENTER_LEFT,
		BOTTOM_LEFT,

		TOP_CENTER,
		CENTER_CENTER,
		BOTTOM_CENTER,

		TOP_RIGHT,
		CENTER_RIGHT,
		BOTTOM_RIGHT
	};

private:
	SDL_Renderer* renderer = nullptr;

	TTF_Font* font = nullptr;

	// controls how coordinates are interpreted (see applyCanvasAlignment) (e.g. TOP_LEFT means that the coord supplied will be the top left of the rendered object)
	Alignment alignment = Alignment::TOP_LEFT;

	// coorrdinates are interpreted as relative to this point
	int originX = 0, originY = 0;

	// why not just set the sdl instead of storing it? because if someone sets the canvas color, then manually sets the sdl color, we should only use the original color they gave to the canvas. so we cannot rely on sdl to store it for us
	SDL_Color color = { 255,255,255,255 };

public:

	// --- setters ---

	void init(SDL_Renderer* renderer);

	void setFont(TTF_Font* font);

	void setAlignment(Alignment alignment);
	void setOrigin(int x, int y);
	void setOriginToCenter();
	void resetOrigin();

	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setColor(Uint8 c);
	void setColor(Uint8 c, Uint8 a);
	void setColor(SDL_Color c);


	
	// --- getters ---

	int getWidth();
	int getHeight();

	SDL_Color getColor();
	SDL_Renderer* getSDLRenderer(); // TODO: this is terrible (and temporary!). Raw pointer to renderer should not be exposed. 


	// --- draw funtions ---

	void clear();

	void present();

	void drawRect(int x, int y, int w, int h);
	void drawRect(const SDL_Rect& rect);
	void drawRect(int x, int y, int w, int h, int thickness);
	void drawRect(const SDL_Rect&, int thickness);
	void fillRect(int x, int y, int w, int h);
	void fillRect(const SDL_Rect& rect);

	void drawLine(int x1, int y1, int x2, int y2);

	void drawCubicBezier(Vec2D p0, Vec2D p1, Vec2D p2, Vec2D p3, int thickness, int resolution);

	void drawString            (std::string text, int x, int y              );
	void drawStringToWidth     (std::string text, int x, int y, int w       );
	void drawStringToHeight    (std::string text, int x, int y, int h       );
	void drawStringToDimensions(std::string text, int x, int y, int w, int h);
	SDL_Texture* drawStringToTexture(std::string text); // TODO: this returns a raw pointer to an SDL_Texture, which the user of this function must manually destroy using SDL_DestroyTexture(textTexture); --> create a wrapper class that does this automagically
	void getTextDimensions(std::string text, int* w, int* h);
	
	void drawImage(SDL_Texture *image, int x, int y, int w, int h);
	void drawImage(SDL_Texture *image, int x, int y);
	void drawImage_StrechToFillCanvas(SDL_Texture *image); // draw image, stretching to entire canvas
	void drawImageRegion(SDL_Texture* image, const SDL_Rect& region, int x, int y); // draw a part of an image
	void drawImageRegion(SDL_Texture* image, const SDL_Rect& region, int x, int y, int w, int h);

	void drawRoundedRect(int x, int y, int w, int h, int r, int thickness, SDL_Color innerColor, SDL_Color outerColor, const unsigned int trianglesPerCorner=8);
	void fillRoundedRect(int x, int y, int w, int h, int r, const unsigned int trianglesPerCounter = 8);

	void renderRegularPolygon(int x, int y, unsigned int numVertices = 3, float rotAngle = 0, float scale = 250.0f);

private:

	// --- helper methods ---
	
	void setSDLColorToCanvasColor();

	void applyCanvasOrigin(int& x, int& y);
	void applyCanvasAlignment(int &x, int &y, int w, int h);
	HorizontalAlignment getHorizontaAlignment(Alignment alignment);
	VerticalAlignment getVerticalAlignment(Alignment alignment);

	std::vector<SDL_Vertex> getRoundedRectVertices(int x, int y, int w, int h, int r, int trianglesPerCorner, SDL_Color color);
	std::vector<SDL_Vertex> interleaveVertices(const std::vector<SDL_Vertex>& v1, const std::vector<SDL_Vertex>& v2);
	std::vector<int> getIndices_TriangleFan(int numVertices);
	std::vector<int> getIndices_TriangleStrip(int numVertices);
	std::vector<int> getIndices_TriangleStrip_Loop(int numVertices);


};