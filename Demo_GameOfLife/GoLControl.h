#pragma once

#include <unordered_map>

#include "GG/Core/Application.h"


class GoLControl : public GG::Control
{
	SDL_Texture* texture;
	int oldW, oldH;
	bool isDirty;
	int textureW, textureH;


public:
    GoLControl()
    {
        isScrollEventListener = true;
        isMouseEventListener = true;
    }

    void render(GG::Canvas* canvas) override
    {
		// call super
		Element::render(canvas);

		// regenerate texture if the control's area has changed
		isDirty = (w != oldW) || (h != oldH);
		oldW = w;
		oldH = h;
		if (isDirty) generateTexture(canvas);

		// start rendering to texture
		SDL_SetRenderTarget(canvas->getSDLRenderer(), texture);

		renderGoLBoard(canvas);

		// resume rendering to window 
		SDL_SetRenderTarget(canvas->getSDLRenderer(), NULL);

		// render texture to window
		const int imagePadding = 0;
		canvas->drawImage(texture, screenX + borderThickness + imagePadding, screenY + borderThickness + imagePadding, w - 2 * borderThickness - 2 * imagePadding, h - 2 * borderThickness - 2 * imagePadding);

		// render rounded inner frame on top of image to hide its corners
		canvas->drawRoundedRect(screenX, screenY, w, h, radius, 5, borderColor, borderColor, 12);
    }

private:
	void generateTexture(GG::Canvas* canvas)
	{
		if (texture != nullptr)	SDL_DestroyTexture(texture);

		textureW = (w - borderThickness);
		textureH = (h - borderThickness);

		texture = SDL_CreateTexture(canvas->getSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureW, textureH);
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); // when enlarging the texture for rendering, keep things "pixelated"
	}

	void renderGoLBoard(GG::Canvas* canvas)
	{
		// clear screen
		canvas->setColor(60);
		canvas->fillRect(0, 0, textureW, textureH);


		canvas->setColor(255, 0, 0, 255);
		canvas->fillRect(100, 100, 100, 100);
	}

	int rectX = 100, rectY = 100, rectW = 100, rectH = 100;
	bool isDragging = false;
	int cursorRelativePosX, cursorRelativePosY;
	void uiScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override
	{
	}
	void onMouseDown(int mouseX, int mouseY)
	{
		if (GG::MoreMath::isPointInRect(mouseX, mouseY, rectX, rectY, rectW, rectH))
		{
			isDragging = true;
		}

	}
    void onMouseDrag(int mouseX, int mouseY)
	{

	}
	void onMouseUp(int mouseX, int mouseY)
	{
	}
};
