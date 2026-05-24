#pragma once

#include <vector>
#include <optional>

#include "GG/Input/InputManager.h"
#include "GG/Rendering/Canvas.h"
#include "GG/UI/Element.h"

namespace GG
{
	class Control : public MouseEventListener, public KeyEventListener, public TextInputEventListener, public ScrollEventListener, public Element
	{
	public:
		Control();

	public:
		virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;

		virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;

		virtual void onTextInputEvent(std::string text) override;

		virtual void onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override;

		virtual void accept(Visitor& visitor) override;
	};
}
