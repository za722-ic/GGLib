#pragma once

#include <vector>
#include <optional>

#include "InputManager.h"
#include "Canvas.h"
#include "Element.h"


class Control : public MouseEventListener, public KeyEventListener, public TextInputEventListener, public Element
{
public:
	virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override
	{}

	virtual void onTextInputEvent(std::string text) override
	{}

	void accept(Visitor& visitor) override
	{
		visitor.visitForControl(this);
	}
};
