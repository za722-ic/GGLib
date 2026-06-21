#pragma once

#include <functional>

#include "GG/UI/Container.h"
#include "GG/Input/InputManager.h"

namespace GG
{
	class RootContainer : public Container, ScrollEventListener, MouseEventListener, TextInputEventListener, KeyEventListener
	{
	public:
		InputManager* inputManager = nullptr;

		Element* activeScrollListener = nullptr;
		Element* activeTextInputListener = nullptr;
		Element* activeDragListener = nullptr;

		Element* hitElement = nullptr;

		enum class MouseState
		{
			MOUSE_UP,
			MOUSE_DOWN,
		};

		MouseState state = MouseState::MOUSE_UP;

		Container* overlay = nullptr;

	public:
		void calculateLayout(int screenX, int screenY, int screenW, int screenH);

		void setInputManager(InputManager* inputManager);

		void render(Canvas* canvas) override;

		void accept(Visitor& visitor) override;

		void onScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override;
		void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
		void onTextInputEvent(const std::string text) override;
		void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;

	private:
		void subscribeToInputs();
		void unsubscribeFromInputs();
	};
}
