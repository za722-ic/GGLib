#pragma once

#include <memory>

#include "Control.h"

#include "Text.h"

class Label: public Control
{
private:
	std::unique_ptr<Text> text = nullptr;

	int textPadding = 0;

	bool autoSizeHorizontal = false;
	bool autoSizeVertical = false;

public:
	// TODO: make use of width, height --> crop text with "..." if out of bounds (how do other libs handle this?)
	

	Label(std::string textStr = "")
	{
		text = std::make_unique<Text>(textStr);
	}

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	void render(Canvas* canvas) override
	{
		// draw back
		canvas->setColor(color);
		canvas->fillRoundedRect(screenX, screenY, w, h, radius);

		// clip text to these bounds
		SDL_Rect clipRect = { screenX + textPadding, screenY + textPadding, w - 2*textPadding, h-2*textPadding };
		SDL_SetRenderClipRect(canvas->getSDLRenderer(), &clipRect);

		// perform text layout
		text->setWrapWidth(clipRect.w);
		text->setPosition(clipRect.x, clipRect.y);

		// render text
		text->render();

		SDL_SetRenderClipRect(canvas->getSDLRenderer(), NULL);
	}

	void accept(Visitor& visitor) override
	{
		visitor.visitForLabel(this);
	}

	void setText(std::string newText)
	{
		text->setText(newText);
	}
	std::string getText()
	{
		return text->getText();
	}
	
	void setAlignment(TTF_HorizontalAlignment alignment)
	{
		text->setAlignment(alignment);
	}
	void setForeColor(SDL_Color color)
	{
		text->setColor(color.r, color.g, color.b, color.a);
	}
	void setBackColor(SDL_Color color)
	{
		this->color = color;
	}
	void setTextPadding(int textPadding)
	{
		this->textPadding = textPadding;
	}

	void setAutoSizeHorizontal(bool newHorizontalAutosize)
	{
		autoSizeHorizontal = newHorizontalAutosize;
	}
	bool getAutoSizeHorizontal()
	{
		return autoSizeHorizontal;
	}

	void setAutoSizeVertical(bool newVerticalAutosize)
	{
		autoSizeVertical = newVerticalAutosize;
	}
	bool getAutoSizeVertical()
	{
		return autoSizeVertical;
	}

	std::pair<int, int> getTextDimensions()
	{
		return text->getDimensions();
	}
};
