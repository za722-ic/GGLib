#pragma once

#include <functional>

#include "GG/Input/InputManager.h"

class DragManager
{
private:
	enum MouseState
	{
		MOUSE_UP = 0,
		MOUSE_DOWN,
		MOUSE_DOWN_AND_MOVE // aka drag
	};

	MouseState state = MOUSE_UP;

	int prevMouseX = -1;
	int prevMouseY = -1;

	std::function<void(int mouseX, int mouseY)> onClick;
	std::function<void(int mouseX, int mouseY)> onDragStart;
	std::function<void(int mouseX, int mouseY)> onDragContinue;
	std::function<void(int mouseX, int mouseY)> onDragEnd;

public:
	void processDrag(MouseEventType mouseEventType, int mouseX, int mouseY);

	void setOnClick(std::function<void(int mouseX, int mouseY)> newOnClick); // called when mouse released without moving

	void setOnDragStart(std::function<void(int mouseX, int mouseY)> newOnDragStart); // called first time mouse is moved whilst down

	void setOnDragContinue(std::function<void(int mouseX, int mouseY)> newOnDragContinue); // called each subsequent time mouse is moved whilst down after onDragStart

	void setOnDragEnd(std::function<void(int mouseX, int mouseY)> newOnDragEnd); // called when mouse released after moving it whilst mouse down
};
