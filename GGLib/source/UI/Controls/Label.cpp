#include "GG/UI/Controls/Label.h"


Label::Label(std::string textStr)
{
	tttext = std::make_unique<Text>(textStr);
}

void Label::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void Label::accept(Visitor& visitor)
{
	visitor.visitForLabel(this);
}

void Label::render(Canvas* canvas)
{
	// draw back
	canvas->setColor(color);
	canvas->fillRoundedRect(screenX, screenY, w, h, radius);

	// clip text to these bounds
	SDL_Rect clipRect = { screenX + paddingRight, screenY + paddingTop, w - paddingLeft - paddingRight, h - paddingTop - paddingBottom };

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

void Label::setText(std::string newText)
{
	tttext->setText(newText);
}
std::string Label::getText()
{
	return tttext->getText();
}

void Label::setHAlignment(HAlignmentMode alignment)
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

void Label::setVAlignment(VAlignmentMode alignment)
{
	verticalAlignmentMode = alignment;
}

void Label::setForeColor(SDL_Color color)
{
	tttext->setColor(color.r, color.g, color.b, color.a);
}
void Label::setBackColor(SDL_Color color)
{
	this->color = color;
}

std::pair<int, int> Label::getTextDimensions()
{
	return tttext->getDimensions();
}
