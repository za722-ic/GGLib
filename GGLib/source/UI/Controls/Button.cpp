#include "GG/UI/Controls/Button.h"

// TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)

GG::Button::Button(std::string text) : text(text)
{}

// bounds
void GG::Button::setBounds(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

// color
void GG::Button::setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	foreColor = { r, g, b, a };
}
void GG::Button::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = { r, g, b, a };
}

// text
void GG::Button::setText(std::string _text)
{
	text = _text;
}
std::string GG::Button::getText() const
{
	return text;
}

// button events
void GG::Button::setOnClick(std::function<void()> func)
{
	onClick = func;
}
void GG::Button::setOnMouseEnter(std::function<void()> func)
{
	onMouseEnter = func;
}
void GG::Button::setOnMouseExit(std::function<void()> func)
{
	onMouseExit = func;
}

void GG::Button::render(GG::Canvas* canvas)
{
	// draw button body
	if (isMouseDown)
	{
		canvas->setColor(backColorOnMouseDown);
	}
	else if (isInBounds)
	{
		canvas->setColor(backColorOnHover);
	}
	else
	{
		canvas->setColor(backColor);
	}
	canvas->fillRoundedRect(screenX, screenY, w, h, radius);

	// draw button border and shadow
	int borderRadius = radius + borderThickness;
	int shadowRadius = borderRadius + shadowThickness;
	canvas->drawRoundedRect(screenX - borderThickness,
		screenY - borderThickness,
		w + 2 * borderThickness,
		h + 2 * borderThickness, borderRadius, borderThickness, borderColor, borderColor);

	canvas->drawRoundedRect(screenX - borderThickness - shadowThickness,
		screenY - borderThickness - shadowThickness,
		w + 2 * borderThickness + 2 * shadowThickness,
		h + 2 * borderThickness + 2 * shadowThickness,
		shadowRadius,
		shadowThickness,
		{ 0,0,0,96 }, { 0,0,0,0 });


	// draw button text
	canvas->setColor(foreColor);
	canvas->setAlignment(GG::Canvas::Alignment::CENTER_CENTER);
	canvas->drawString(text, screenX + w/2, screenY + h / 2);
	canvas->setAlignment(GG::Canvas::Alignment::TOP_LEFT);
}

// mouse events
void GG::Button::onMouseEvent(GG::MouseEventType mouseEventType, int mouseX, int mouseY)
{
	wasInBounds = isInBounds;
	isInBounds = (mouseX >= screenX && mouseX < screenX + w) && (mouseY >= screenY && mouseY < screenY + h);


	if (isInBounds)
	{
		if (mouseEventType == GG::MouseEventType::LEFT_MOUSE_DOWN)
		{
			isMouseDown = true;
		}
		else if (mouseEventType == GG::MouseEventType::LEFT_MOUSE_UP)
		{
			isMouseDown = false;
			if (onClick) onClick();
		}
	}
	else
	{
		isMouseDown = false;
	}

	if (!wasInBounds && isInBounds && mouseEventType == GG::MouseEventType::MOUSE_MOVE && onMouseEnter)
	{
		onMouseEnter();
	}

	if (wasInBounds && !isInBounds && mouseEventType == GG::MouseEventType::MOUSE_MOVE && onMouseExit)
	{
		onMouseExit();
	}
}
