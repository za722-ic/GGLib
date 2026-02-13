#pragma once

#include "UIControl.h"

class Combobox : UIControl
{
protected:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	SDL_Color backColor = {200, 200, 200, 255};
	SDL_Color foreColor = { 0, 0, 0, 255 };

	std::string text = "Select option";

	std::vector<std::string> options = { "A", "B", "C" };

public:

	// bounds
	void setBounds(int _x, int _y, int _w, int _h);

	// color
	void setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	void setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	// text
	void setText(std::string _text);
	std::string getText() const;

	void render(Canvas *canvas) override;

private:

	// mouse events
	bool isExpanded = false;
	int indexToHighlight = -1;
	bool isInBounds = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
};
