#pragma once

#include <vector>
#include <optional>

#include "GG/Input/InputManager.h"
#include "GG/Rendering/Canvas.h"
#include "GG/UI/Element.h"
#include "GG/UI/Control.h"

GG::Control::Control()
{
}

void GG::Control::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void GG::Control::onKeyEvent(KeyEventType keyEventType, SDL_Keycode key)
{
}

void GG::Control::onTextInputEvent(std::string text)
{
}

void GG::Control::onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY)
{
}

void GG::Control::accept(Visitor& visitor)
{
	visitor.visitForControl(this);
}
