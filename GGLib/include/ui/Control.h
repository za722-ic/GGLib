#pragma once

#include <vector>
#include <optional>

#include "InputManager.h"
#include "Canvas.h"
#include "Element.h"

#include "Text.h"

class Control : public MouseEventListener, public KeyEventListener, public TextInputEventListener, public ScrollEventListener, public Element
{
public:
	std::unique_ptr<Text> tttext = nullptr;
	bool labAutosize = false;

	Control(std::string textStr = "")
	{
		tttext = std::make_unique<Text>(textStr);
	}

public:
	virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{}

	virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override
	{}

	virtual void onTextInputEvent(std::string text) override
	{}

	virtual void onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override
	{}

	virtual void accept(Visitor& visitor) override
	{
		visitor.visitForControl(this);
	}
};
