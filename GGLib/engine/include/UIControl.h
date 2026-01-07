#pragma once

#include <vector>

#include "InputManager.h"
#include "Canvas.h"

class UIControl : MouseEventListener
{
// TODO: create explicit init method instead of just having this public
public:
	static InputManager *inputManager;
	static std::vector<UIControl*> uiControls;

	UIControl();
	~UIControl(); // TODO: rule of three

	virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
	{}

	virtual void render(Canvas* canvas)
	{}
};
