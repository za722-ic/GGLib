#pragma once

#include <memory>

#include "ui/Control.h"
#include "ui/UITypes.h"
#include "Text.h"


class Label: public Control
{
public:
	std::unique_ptr<Text> tttext = nullptr;

	VAlignmentMode verticalAlignmentMode = VAlignmentMode::TOP;

public:
	// TODO: make use of width, height --> crop text with "..." if out of bounds (how do other libs handle this?)

	Label(std::string textStr = "") 
	{
		tttext = std::make_unique<Text>(textStr);
	}

	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	void accept(Visitor& visitor) override
	{
		visitor.visitForLabel(this);
	}

	void render(Canvas* canvas) override
	{
		// draw back
		canvas->setColor(color);
		canvas->fillRoundedRect(screenX, screenY, w, h, radius);

		// clip text to these bounds
		SDL_Rect clipRect = { screenX + paddingRight, screenY + paddingTop, w - paddingLeft - paddingRight, h-paddingTop-paddingBottom };

		if (clipRect.w <= 0 || clipRect.h <= 0) return;

		SDL_SetRenderClipRect(canvas->getSDLRenderer(), &clipRect);
		{
			int textY;
			if (verticalAlignmentMode == VAlignmentMode::TOP)
			{
				textY = clipRect.y;
			}
			else if (verticalAlignmentMode == VAlignmentMode::CENTER)
			{
				int textH = tttext->getDimensions().second;
				textY = clipRect.y + clipRect.h / 2 - textH / 2;
			}
			else if (verticalAlignmentMode == VAlignmentMode::BOTTOM)
			{
				int textH = tttext->getDimensions().second;
				textY = clipRect.y + clipRect.h - textH;
			}
			else
			{
				assert(false);
			}

			// text layout
			tttext->setPosition(clipRect.x, textY);

			// render text
			tttext->render();

		}
		SDL_SetRenderClipRect(canvas->getSDLRenderer(), NULL);
	}

	void setText(std::string newText)
	{
		tttext->setText(newText);
	}
	std::string getText()
	{
		return tttext->getText();
	}
	
	void setHAlignment(HAlignmentMode alignment)
	{
		switch (alignment)
		{
		case HAlignmentMode::LEFT:
			tttext->setHAlignment(TTF_HORIZONTAL_ALIGN_LEFT);
			break;
		case HAlignmentMode::CENTER:
			tttext->setHAlignment(TTF_HORIZONTAL_ALIGN_CENTER);
			break;
		case HAlignmentMode::RIGHT:
			tttext->setHAlignment(TTF_HORIZONTAL_ALIGN_RIGHT);
			break;

		default:
			assert(false); // something wrong has happened
		}
	}

	void setVAlignment(VAlignmentMode alignment)
	{
		verticalAlignmentMode = alignment;
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
};
