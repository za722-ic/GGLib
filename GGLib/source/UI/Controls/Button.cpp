#include "GG/UI/Controls/Button.h"

// TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)

GG::Button::Button(std::string text) : text(text)
{
	isMouseEventListener = true;
}

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
	onClickCallback = func;
}
void GG::Button::setOnMouseEnter(std::function<void()> func)
{
	onMouseEnterCallback = func;
}
void GG::Button::setOnMouseExit(std::function<void()> func)
{
	onMouseExitCallback = func;
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

	// TODO: should border instead be AT screenX, screenY? (this applies to a whole bunch of other elements, including the Element class's own render function)
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

void GG::Button::onMouseDown(int mouseX, int mouseY)
{
	isMouseDown = true;
}

void GG::Button::onMouseUp(int mouseX, int mouseY)
{
	isMouseDown = false;
	if (onClickCallback) onClickCallback();
}

void GG::Button::onMouseEnter(int mouseX, int mouseY)
{
	isInBounds = true;
	if (onMouseEnterCallback) onMouseEnterCallback();
}

void GG::Button::onMouseExit(int mouseX, int mouseY)
{
	isInBounds = false;
	if (onMouseExitCallback) onMouseExitCallback();
}
