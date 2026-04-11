#include "ui/Button.h"

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

void Button::render(Canvas *canvas)
{
	canvas->setColor(backColor);
	canvas->fillRect(screenX, screenY, w, h);

	canvas->setColor(foreColor);
	canvas->setAlignment(Canvas::Alignment::CENTER_CENTER);
	canvas->drawString(text, screenX + w / 2, screenY + h / 2);
	canvas->setAlignment(Canvas::Alignment::TOP_LEFT); // TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)
}

// mouse events
void Button::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	wasInBounds = isInBounds;
	isInBounds = (mouseX >= screenX && mouseX < screenX + w) && (mouseY >= screenY && mouseY < screenY + h);

	if (isInBounds && mouseEventType == MouseEventType::LEFT_MOUSE_UP && onClick)
	{
		onClick();
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
