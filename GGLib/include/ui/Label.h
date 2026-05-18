#pragma once

#include <memory>

#include "Control.h"

#include "Text.h"

class Label: public Control
{
public:
	// TODO: make use of width, height --> crop text with "..." if out of bounds (how do other libs handle this?)
	

	Label(std::string textStr = "") : Control(textStr)
	{
		//verticalAutosize = horizontalAutosize = true;
		//verticalAutosize = true;
		color = { 255,0,0,128 };
		radius = 0;

		updatePreferredDimensions();
	}

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	void render(Canvas* canvas) override
	{
		// draw back
		canvas->setColor(color);
		canvas->fillRoundedRect(screenX, screenY, w, h, radius);

		// clip text to these bounds
		SDL_Rect clipRect = { screenX + paddingRight, screenY + paddingTop, w - paddingLeft - paddingRight, h-paddingTop-paddingBottom };

		if (clipRect.w <= 0 || clipRect.h <= 0) return;

		SDL_SetRenderClipRect(canvas->getSDLRenderer(), &clipRect);

		// perform text layout
		//text->setWrapWidth(clipRect.w);
		tttext->setPosition(clipRect.x, clipRect.y);

		// render text
		tttext->render();

		SDL_SetRenderClipRect(canvas->getSDLRenderer(), NULL);
	}

	void setText(std::string newText)
	{
		tttext->setText(newText);
		updatePreferredDimensions();
	}
	std::string getText()
	{
		return tttext->getText();
	}
	
	void setAlignment(TTF_HorizontalAlignment alignment)
	{
		tttext->setAlignment(alignment);
		updatePreferredDimensions();
	}
	void setForeColor(SDL_Color color)
	{
		tttext->setColor(color.r, color.g, color.b, color.a);
	}
	void setBackColor(SDL_Color color)
	{
		this->color = color;
	}

	std::pair<int, int> getTextDimensions()
	{
		return tttext->getDimensions();
	}

	
	

	void updatePreferredDimensions()
	{
		auto dim = tttext->getDimensions();
		setPreferredWidth(dim.first);
		setPreferredHeight(dim.second);
	}
};
