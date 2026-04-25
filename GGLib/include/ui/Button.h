#pragma once

// TODO: this is "dumb" pointer havoc below --> please clean up
// TODO: the button class should inherit from some "control" class that removes some of the generic mouseeventlistener stuff, and that stipulates rendering, and other stuff
// TODO: what if some other button (or cvontrol in general) is above this button? then it should not receive mouse events --> they should instead go to the topmost at that location --> need to maintain z order (the last in the list of the paren't controls has the z closest to the user). Also deal with partial occlusion

#include <functional>

#include "Control.h"

class Button : public Control
{
public:
	std::function<void()> onClick;
	std::function<void()> onMouseEnter;
	std::function<void()> onMouseExit;

	SDL_Color backColorOnHover = {200,200,200, 255};
	SDL_Color backColorOnMouseDown = { 180,180,180,255 };
	SDL_Color backColor = { 230,230,230,255 };
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
	bool isMouseDown = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
};
