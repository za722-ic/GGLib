#pragma once

#include <vector>

#include "InputManager.h"
#include "Canvas.h"

class UIControl : public MouseEventListener, public KeyEventListener, public TextInputEventListener
{
public:

	virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override
	{}

	virtual void onTextInputEvent(std::string text) override
	{}

	virtual void render(Canvas* canvas)
	{}
};
