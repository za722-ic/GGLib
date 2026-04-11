#pragma once

#include "Control.h"

class Label: public Control
{
public:
	// TODO: make use of width, height --> crop text with "..." if out of bounds (how do other libs handle this?)
	
	int textPadding;

	std::string text;

	SDL_Color foreColor;

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	void render(Canvas* canvas) override
	{
		canvas->setColor(foreColor);

		canvas->drawString(text, screenX, screenY);
	}

	void setText(std::string newText)
	{
		text = newText;
	}
};
