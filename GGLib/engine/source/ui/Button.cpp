#include "ui/Button.h"

// TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)

// bounds
void Button::setBounds(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

// color
void Button::setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	foreColor = { r, g, b, a };
}
void Button::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = { r, g, b, a };
}

// text
void Button::setText(std::string _text)
{
	text = _text;
}
std::string Button::getText() const
{
	return text;
}

// button events
void Button::setOnClick(std::function<void()> func)
{
	onClick = func;
}
void Button::setOnMouseEnter(std::function<void()> func)
{
	onMouseEnter = func;
}
void Button::setOnMouseExit(std::function<void()> func)
{
	onMouseExit = func;
}

void Button::render(Canvas* canvas)
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
	canvas->drawString(text, screenX + paddingLeft, screenY + paddingTop);
}

// mouse events
void Button::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	wasInBounds = isInBounds;
	isInBounds = (mouseX >= screenX && mouseX < screenX + w) && (mouseY >= screenY && mouseY < screenY + h);


	if (isInBounds)
	{
		if (mouseEventType == MouseEventType::LEFT_MOUSE_DOWN)
		{
			isMouseDown = true;
		}
		else if (mouseEventType == MouseEventType::LEFT_MOUSE_UP)
		{
			isMouseDown = false;
			if (onClick) onClick();
		}
	}
	else
	{
		isMouseDown = false;
	}

	if (!wasInBounds && isInBounds && mouseEventType == MouseEventType::MOUSE_MOVE && onMouseEnter)
	{
		onMouseEnter();
	}

	if (wasInBounds && !isInBounds && mouseEventType == MouseEventType::MOUSE_MOVE && onMouseExit)
	{
		onMouseExit();
	}
}
