#pragma once

#include "UIControl.h"

class Label: UIControl
{
public:
	int x, y;
	int w, h; // TODO: make use of these --> crop text with ... if out of bounds (how do other libs handle this?)
	int textPadding;

	std::string text;

	SDL_Color foreColor;

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	void render(Canvas* canvas) override
	{
		canvas->setColor(foreColor);

		canvas->drawString(text, x, y);
	}

	void setText(std::string newText)
	{
		text = newText;
	}
};
