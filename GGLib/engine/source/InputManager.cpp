#include "InputManager.h"

void InputManager::init(SDL_Renderer* newRenderer)
{
	SDL_StartTextInput();
	renderer = newRenderer;
}

bool InputManager::pollEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		// quit requested (via application window "X" press)
		if (e.type == SDL_QUIT)
			return false;

		// mouse events
		if (e.type == SDL_MOUSEMOTION)
			notifyMouseEventListeners(MOUSE_MOVE);
		if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
			notifyMouseEventListeners(LEFT_MOUSE_DOWN);
		if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			notifyMouseEventListeners(LEFT_MOUSE_UP);
		if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT)
			notifyMouseEventListeners(RIGHT_MOUSE_DOWN);
		if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
			notifyMouseEventListeners(RIGHT_MOUSE_UP);

		// mouse scroll event
		if (e.type == SDL_MOUSEWHEEL)
		{
			notifyScrollEventListeners(e.wheel.mouseX, e.wheel.mouseY, e.wheel.preciseX, e.wheel.preciseY);
		}

		// key board events
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.repeat == 0)
			{
				notifyKeyEventListeners(KEY_DOWN, e.key.keysym.sym);
			}
			else
			{
				notifyKeyEventListeners(KEY_DOWN_REPEAT, e.key.keysym.sym); // if this key has been pressed and held, therefore triggering multiple events
			}
		}
		if (e.type == SDL_KEYUP)
			notifyKeyEventListeners(KEY_UP, e.key.keysym.sym);

		// text input events
		if (e.type == SDL_TEXTINPUT)
		{
			notifyTextInputEventListeners(std::string(e.text.text));
		}
	}
	return true;
}

void InputManager::addKeyEventListener(KeyEventListener* keyEventListener)
{
	keyEventListeners.push_back(keyEventListener);
}
void InputManager::removeKeyEventListener(KeyEventListener* keyEventListener)
{
	keyEventListeners.remove(keyEventListener);
}

void InputManager::addMouseEventListener(MouseEventListener* mouseEventListener)
{
	mouseEventListeners.push_back(mouseEventListener);
}
void InputManager::removeMouseEventListener(MouseEventListener* mouseEventListener)
{
    mouseEventListeners.remove(mouseEventListener);
}

void InputManager::addTextInputEventListener(TextInputEventListener* textInputEventListener)
{
	textInputEventListeners.push_back(textInputEventListener);
}

void InputManager::removeTextInputEventListener(TextInputEventListener* textInputEventListener)
{
	textInputEventListeners.remove(textInputEventListener);
}

void InputManager::addScrollEventListener(ScrollEventListener* scrollEventListener)
{
	scrollEventListeners.push_back(scrollEventListener);
}

void InputManager::removeScrollEventListener(ScrollEventListener* scrollEventListener)
{
	scrollEventListeners.remove(scrollEventListener);
}

const Uint8* InputManager::getKeyboardState()
{
	SDL_PumpEvents(); // NOTE: as per the documentation here: https://wiki.libsdl.org/SDL2/SDL_GetKeyboardState I should cal SDL_PumpEvents before SDL_GetKeyboardState. But as per here: https://wiki.libsdl.org/SDL2/SDL_PumpEvents SDL_PollEvent() above already calls this implicitly. I do it here anyway just to be safe.
	return SDL_GetKeyboardState(NULL);
}

int InputManager::getMouseX()
{
	return mouseX;
}

int InputManager::getMouseY()
{
	return mouseY;
}

void InputManager::notifyKeyEventListeners(KeyEventType keyEventType, SDL_Keycode key)
{
	for (KeyEventListener* keyEventListenerPtr : keyEventListeners)
	{		
		keyEventListenerPtr->onKeyEvent(keyEventType, key);
	}
}

void InputManager::notifyMouseEventListeners(MouseEventType mouseEventType)
{
	pollMousePosition();

	for (MouseEventListener* mouseEventListenerPtr : mouseEventListeners)
	{
		mouseEventListenerPtr->onMouseEvent(mouseEventType, mouseX, mouseY);
	}
}

void InputManager::notifyTextInputEventListeners(std::string text)
{
	for (TextInputEventListener* textInputEventListenerPtr : textInputEventListeners)
	{		
		textInputEventListenerPtr->onTextInputEvent(text);
	}
}

void InputManager::notifyScrollEventListeners(int mouseX, int mouseY, float scrollX, float scrollY)
{
	for (ScrollEventListener* scrollEventListenerPtr: scrollEventListeners)
	{		
		scrollEventListenerPtr->onScrollEvent(mouseX, mouseY, scrollX, scrollY);
	}
}

void InputManager::pollMousePosition()
{
	int windowMouseX, windowMouseY;
	SDL_GetMouseState(&windowMouseX, &windowMouseY);

	float logicalMouseX, logicalMouseY;
	SDL_RenderWindowToLogical(renderer, windowMouseX, windowMouseY, &logicalMouseX, &logicalMouseY);

	mouseX = (int)logicalMouseX;
	mouseY = (int)logicalMouseY;
}
