#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>

#include "GG/MoreMath/Vec2D.h"
#include "GG/MoreMath/MoreMath.h"

namespace GG
{
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

		// why not just set the sdl renderer color instead of storing it? because if someone sets the canvas color, then manually sets the sdl color, we should only use the original color they gave to the canvas. so we cannot rely on sdl to store it for us
		SDL_Color color = { 255,255,255,255 };

		// maintain a stack of clip rects. the active clip rect is the region of intersection of all of these.
		std::vector<SDL_Rect> clipRectStack;
		SDL_Rect activeClipRect; // TODO: optimisation: if this is equal to clipRectForNoRendering, then we can cull entire render operations
		const SDL_Rect clipRectForNoRendering = { -1,-1,1,1 }; // by setting the active cliprect to this, rendering is effectively disabled

	public:

		/*
		+----------------------------------+
		|		MISC                       |
		+----------------------------------+
		*/
		void clear();
		void present();
		void init(SDL_Renderer* renderer);



		/*
		+----------------------------------+
		|		SETTERS                    |
		+----------------------------------+
		*/
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



		/*
		+----------------------------------+
		|		GETTERS                    |
		+----------------------------------+
		*/
		// TODO: do these even work? Compare against GG::Window width/height (and whatever else I have) --> there should just be one source of truth
		int getWidth();
		int getHeight();

		SDL_Color getColor();
		SDL_Renderer* getSDLRenderer(); // TODO: this is terrible (and temporary!). Raw pointer to renderer should not be exposed. 



		/*
		+----------------------------------+
		|		TEXT                       |
		+----------------------------------+
		*/
		void drawString(std::string text, int x, int y);
		void drawStringToWidth(std::string text, int x, int y, int w);
		void drawStringToHeight(std::string text, int x, int y, int h);
		void drawStringToDimensions(std::string text, int x, int y, int w, int h);
		SDL_Texture* drawStringToTexture(std::string text); // TODO: this returns a raw pointer to an SDL_Texture, which the user of this function must manually destroy using SDL_DestroyTexture(textTexture); --> create a wrapper class that does this automagically
		void getTextDimensions(std::string text, int* w, int* h);



		/*
		+----------------------------------+
		|		IMAGES                     |
		+----------------------------------+
		*/
		void drawImage(SDL_Texture* image, int x, int y, int w, int h);
		void drawImage(SDL_Texture* image, int x, int y);
		void drawImage_StrechToFillCanvas(SDL_Texture* image); // draw image, stretching to entire canvas
		void drawImageRegion(SDL_Texture* image, const SDL_FRect& region, int x, int y); // draw a part of an image
		void drawImageRegion(SDL_Texture* image, const SDL_FRect& region, int x, int y, int w, int h);



		/*
		+----------------------------------+
		|		SHAPES                     |
		+----------------------------------+
		*/
		void drawRoundedRect(int x, int y, int w, int h, int r, int thickness, SDL_Color innerColor, SDL_Color outerColor, const unsigned int trianglesPerCorner = 8);
		void fillRoundedRect(int x, int y, int w, int h, int r, const unsigned int trianglesPerCounter = 8);

		// TODO: use this to draw/fill regular polygons, circles, ellipses
		// TODO: use applyCanvasOrigin, applyCanvasAlignment in _all_ drawing functions
		// TODO: mark all draw functions as const
		// TODO: itd be nice if we could actually draw the chord, or draw lines to the center (so like an outline of fillArc, rather than strictly just an arc -- see p5.js' arc function's MODE parameter)
		void drawArc(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, int thickness = 4);
		void fillArc(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, bool isChord = false);

		// TODO: remove
		void renderRegularPolygon(int x, int y, unsigned int numVertices = 3, float rotAngle = 0, float scale = 250.0f);

		void drawRect(int x, int y, int w, int h);
		void drawRect(const SDL_Rect& rect);
		void drawRect(int x, int y, int w, int h, int thickness);
		void drawRect(const SDL_Rect&, int thickness);
		void fillRect(int x, int y, int w, int h);
		void fillRect(const SDL_Rect& rect);

		void drawLine(int x1, int y1, int x2, int y2);

		void drawCubicBezier(Vec2D p0, Vec2D p1, Vec2D p2, Vec2D p3, int thickness, int numSegments); // more segments == smoother curves and worse performance



		/*
		+----------------------------------+
		|		CLIP RECTS                 |
		+----------------------------------+
		*/
		void pushClipRect(const SDL_Rect& clipRect);
		void popClipRect();
		void calculateActiveClipRect();

	private:

		// --- helper methods ---

		void setSDLColorToCanvasColor();

		void applyCanvasOrigin(int& x, int& y);
		void applyCanvasAlignment(int& x, int& y, int w, int h);
		HorizontalAlignment getHorizontaAlignment(Alignment alignment);
		VerticalAlignment getVerticalAlignment(Alignment alignment);

		std::vector<SDL_Vertex> getRoundedRectVertices(int x, int y, int w, int h, int r, int trianglesPerCorner, SDL_Color color); // TODO: rename to generateRoundedRectVertices

		// TODO: refactor the implementation of these two functions as there is a lot of duplicate code
		std::vector<SDL_Vertex> generateArcVertices(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, bool isChord);
		std::vector<SDL_Vertex> generateArcNormals(int x, int y, int w, int h, float startAngle, float endAngle, int numTriangles, int thickness);

		std::vector<SDL_Vertex> interleaveVertices(const std::vector<SDL_Vertex>& v1, const std::vector<SDL_Vertex>& v2);
		std::vector<int> getIndices_TriangleFan(int numVertices);
		std::vector<int> getIndices_TriangleStrip(int numVertices);
		std::vector<int> getIndices_TriangleStrip_Loop(int numVertices);

		Vec2D deCasteljau_Cubic(const GG::Vec2D& p0, const GG::Vec2D& p1, const GG::Vec2D& p2, const GG::Vec2D& p3, const float t);
		Vec2D deCasteljau_Cubic_Derivative(const GG::Vec2D& p0, const GG::Vec2D& p1, const GG::Vec2D& p2, const GG::Vec2D& p3, const float t);
	};
}