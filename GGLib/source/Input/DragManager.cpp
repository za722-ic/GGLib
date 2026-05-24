#include "GG/Input/DragManager.h"

void DragManager::processDrag(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	bool mouseMoved = (mouseX != prevMouseX) || (mouseY != prevMouseY);

	if (state == MouseState::MOUSE_UP)
	{
		if (mouseEventType == LEFT_MOUSE_DOWN)
		{
			state = MouseState::MOUSE_DOWN;
			return;
		}
	}

	if (state == MouseState::MOUSE_DOWN)
	{
		if (mouseEventType == LEFT_MOUSE_UP)
		{
			if (onClick) onClick(mouseX, mouseY);
			state = MouseState::MOUSE_UP;
			return;
		}
		if (mouseMoved)
		{
			if (onDragStart) onDragStart(mouseX, mouseY);
			state = MouseState::MOUSE_DOWN_AND_MOVE;
			return;
		}
	}

	if (state == MouseState::MOUSE_DOWN_AND_MOVE)
	{
		if (mouseEventType == LEFT_MOUSE_UP)
		{
			if (onDragEnd) onDragEnd(mouseX, mouseY);
			state = MouseState::MOUSE_UP;
			return;
		}
		if (mouseMoved)
		{
			if (onDragContinue) onDragContinue(mouseX, mouseY);
			state = MouseState::MOUSE_DOWN_AND_MOVE; // redundant technincally as we stay in the same state, but keep for clarity
			return;
		}
	}
}

void DragManager::setOnClick(std::function<void(int mouseX, int mouseY)> newOnClick) // called when mouse released without moving
{
	onClick = newOnClick;
}

void DragManager::setOnDragStart(std::function<void(int mouseX, int mouseY)> newOnDragStart) // called first time mouse is moved whilst down
{
	onDragStart = newOnDragStart;
}

void DragManager::setOnDragContinue(std::function<void(int mouseX, int mouseY)> newOnDragContinue) // called each subsequent time mouse is moved whilst down after onDragStart
{
	onDragContinue = newOnDragContinue;
}

void DragManager::setOnDragEnd(std::function<void(int mouseX, int mouseY)> newOnDragEnd) // called when mouse released after moving it whilst mouse down
{
	onDragEnd = newOnDragEnd;
}
