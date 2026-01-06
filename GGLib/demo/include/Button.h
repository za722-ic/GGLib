#pragma once

// TODO: this is "dumb" pointer havoc below --> please clean up
// TODO: the button class should inherit from some "control" class that removes some of the generic mouseeventlistener stuff, and that stipulates rendering, and other stuff

#include <functional>

#include "Game.h"

class Button : MouseEventListener
{
private:
	std::function<void()> onClick;
	std::function<void()> onMouseEnter;
	std::function<void()> onMouseExit;

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	unsigned char r = 128;
	unsigned char g = 128;
	unsigned char b = 128;

public:

	// std::vector<Button*> buttons;
	InputManager* inputManager;

	Button(InputManager* _inputManager)
	{
		// buttons.push_back(this);
		inputManager = _inputManager;
		inputManager->addMouseEventListener(this);
	}

	// ~Button()
	// {
	// 	auto it = std::find(buttons.begin(), buttons.end(), this);
	// 	buttons.erase(it);
	// }


	// bounds
	void setBounds(int _x, int _y, int _w, int _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	// color
	void setColor(int _r, int _g, int _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	// button events
	void setOnClick(std::function<void()> func)
	{
		onClick = func;
	}
	void setOnMouseEnter(std::function<void()> func)
	{
		onMouseEnter = func;
	}
	void setOnMouseExit(std::function<void()> func)
	{
		onMouseExit = func;
	}

	void render(Canvas *canvas)
	{
		canvas->setColor(r, g, b);
		canvas->fillRect(x, y, w, h);
	}

private:

	// mouse events
	bool wasInBounds = false; // on prev call
	bool isInBounds = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
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
};
