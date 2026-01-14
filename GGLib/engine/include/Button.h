#pragma once

// TODO: this is "dumb" pointer havoc below --> please clean up
// TODO: the button class should inherit from some "control" class that removes some of the generic mouseeventlistener stuff, and that stipulates rendering, and other stuff

#include <functional>

#include "UIControl.h"

class Button : UIControl
{
protected:
	std::function<void()> onClick;
	std::function<void()> onMouseEnter;
	std::function<void()> onMouseExit;

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	SDL_Color backColor = {200, 200, 200, 255};
	SDL_Color foreColor = { 0, 0, 0, 255 };

	std::string text = "";


public:

	// bounds
	void setBounds(int _x, int _y, int _w, int _h);

	// color
	void setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	// text
	void setText(std::string _text);
	std::string getText() const;

	// button events
	void setOnClick(std::function<void()> func);
	void setOnMouseEnter(std::function<void()> func);
	void setOnMouseExit(std::function<void()> func);

	void render(Canvas *canvas) override;

private:

	// mouse events
	bool wasInBounds = false; // on prev call
	bool isInBounds = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
};
