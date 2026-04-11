#pragma once

#include <algorithm>

#include "Control.h"

/*
TODO:
- handle higlighting text (with both mouse and keyboard)
- copying/pasting text (via keyboard shortcut and via context menu)
- scrollbar for textboxes
- multiline textboxes, richtext??
- setting text alignment (left, middle, right)
- support insert key?
*/


class Textbox : Control
{
protected:

	SDL_Color backColor = {200, 200, 200, 255};
	SDL_Color foreColor = { 255,255,255, 255 };

	std::string text = "";

	bool isFocused = false;

	unsigned int PADDING = 7;
	size_t cursorPos = 0;
	unsigned int viewportX = 0;
	unsigned int cursorX = PADDING;

	bool forceShiftViewportLeft = false;
	char erasedChar;
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
	void render(Canvas *canvas) override;


private:

	// mouse events
	bool isInBounds = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;

	// keyboard events
	virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;

	// text input events
	virtual void onTextInputEvent(std::string text) override;

	// handling text
	void insertTextAtCursorPos(std::string text);
	void deleteCharAtCursorPos();
	void deleteCharBeforeCursorPos();
	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorToStartOfPrevToken();
	void moveCursorToStartOfNextToken();
	void moveCursorToStartOfText();
	void moveCursorToEndOfText();
};
