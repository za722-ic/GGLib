#pragma once

#include <vector>
#include <optional>

#include "GG/Input/InputManager.h"
#include "GG/Rendering/Canvas.h"
#include "GG/UI/Element.h"

Control::Control()
{
}

void Control::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void Control::onKeyEvent(KeyEventType keyEventType, SDL_Keycode key)
{
}

void Control::onTextInputEvent(std::string text)
{
}

void Control::onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY)
{
}

void Control::accept(Visitor& visitor)
{
	visitor.visitForControl(this);
}
