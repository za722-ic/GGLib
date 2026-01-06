#include "Button.h"

InputManager* Button::inputManager = nullptr;

Button::Button()
{
	inputManager->addMouseEventListener(this);
}

Button::~Button()
{
	inputManager->removeMouseEventListener(this);
}

// bounds
void Button::setBounds(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

// color
void Button::setColor(int _r, int _g, int _b)
{
	r = _r;
	g = _g;
	b = _b;
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
	canvas->setColor(r, g, b);
	canvas->fillRect(x, y, w, h);
}

// mouse events
void Button::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	wasInBounds = isInBounds;
	isInBounds = (mouseX >= x && mouseX < x + w) && (mouseY >= y && mouseY < y + h);

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
