#pragma once

// TODO: this is "dumb" pointer havoc below --> please clean up
// TODO: the button class should inherit from some "control" class that removes some of the generic mouseeventlistener stuff, and that stipulates rendering, and other stuff

#include <functional>

#include "InputManager.h"
#include "Canvas.h"

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

	static InputManager* inputManager;

	Button();
	~Button();

	// bounds
	void setBounds(int _x, int _y, int _w, int _h);

	// color
	void setColor(int _r, int _g, int _b);

	// button events
	void setOnClick(std::function<void()> func);
	void setOnMouseEnter(std::function<void()> func);
	void setOnMouseExit(std::function<void()> func);

	void render(Canvas* canvas);

private:

	// mouse events
	bool wasInBounds = false; // on prev call
	bool isInBounds = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY);
};
