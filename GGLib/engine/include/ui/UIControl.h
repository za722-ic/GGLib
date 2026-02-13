#pragma once

#include <vector>

#include "InputManager.h"
#include "Canvas.h"

class UIControl : MouseEventListener, KeyEventListener, TextInputEventListener
{
// TODO: create explicit init method instead of just having this public
public:
	static InputManager *inputManager;
	static std::vector<UIControl*> uiControls;

	static std::vector<std::weak_ptr<InputManager>> _uiControls;

	UIControl();
	~UIControl(); // TODO: rule of three

	virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override
	{}

	virtual void onTextInputEvent(std::string text) override
	{}

	virtual void render(Canvas* canvas)
	{}
};
