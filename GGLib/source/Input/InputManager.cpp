#include "GG/Input/InputManager.h"

void GG::InputManager::init(SDL_Renderer* newRenderer)
{
	window = SDL_GetRenderWindow(newRenderer);
	renderer = newRenderer;

	SDL_StartTextInput(window);
}

GG::InputManager::~InputManager()
{
	SDL_StopTextInput(window);
}

bool GG::InputManager::pollEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		// quit requested (via application window "X" press)
		if (e.type == SDL_EVENT_QUIT)
			return false;

		// mouse events
		if (e.type == SDL_EVENT_MOUSE_MOTION)
			notifyMouseEventListeners(MOUSE_MOVE);
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
			notifyMouseEventListeners(LEFT_MOUSE_DOWN);
		if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
			notifyMouseEventListeners(LEFT_MOUSE_UP);
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_RIGHT)
			notifyMouseEventListeners(RIGHT_MOUSE_DOWN);
		if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_RIGHT)
			notifyMouseEventListeners(RIGHT_MOUSE_UP);

		// mouse scroll event
		if (e.type == SDL_EVENT_MOUSE_WHEEL)
		{
			notifyScrollEventListeners(e.wheel.mouse_x, e.wheel.mouse_y, e.wheel.x, e.wheel.y);
		}

		// key board events
		if (e.type == SDL_EVENT_KEY_DOWN)
		{
			if (e.key.repeat == 0)
			{
				notifyKeyEventListeners(KEY_DOWN, e.key.key);
			}
			else
			{
				notifyKeyEventListeners(KEY_DOWN_REPEAT, e.key.key); // if this key has been pressed and held, therefore triggering multiple events
			}
		}
		if (e.type == SDL_EVENT_KEY_UP)
			notifyKeyEventListeners(KEY_UP, e.key.key);

		// text input events
		if (e.type == SDL_EVENT_TEXT_INPUT)
		{
			notifyTextInputEventListeners(std::string(e.text.text));
		}
	}
	return true;
}

void GG::InputManager::addKeyEventListener(GG::KeyEventListener* keyEventListener)
{
	keyEventListeners.push_back(keyEventListener);
}
void GG::InputManager::removeKeyEventListener(GG::KeyEventListener* keyEventListener)
{
	keyEventListeners.remove(keyEventListener);
}

void GG::InputManager::addMouseEventListener(GG::MouseEventListener* mouseEventListener)
{
	mouseEventListeners.push_back(mouseEventListener);
}
void GG::InputManager::removeMouseEventListener(GG::MouseEventListener* mouseEventListener)
{
	mouseEventListeners.remove(mouseEventListener);
}

void GG::InputManager::addTextInputEventListener(GG::TextInputEventListener* textInputEventListener)
{
	textInputEventListeners.push_back(textInputEventListener);
}

void GG::InputManager::removeTextInputEventListener(GG::TextInputEventListener* textInputEventListener)
{
	textInputEventListeners.remove(textInputEventListener);
}

void GG::InputManager::addScrollEventListener(GG::ScrollEventListener* scrollEventListener)
{
	scrollEventListeners.push_back(scrollEventListener);
}

void GG::InputManager::removeScrollEventListener(GG::ScrollEventListener* scrollEventListener)
{
	scrollEventListeners.remove(scrollEventListener);
}

const Uint8* GG::InputManager::getKeyboardState()
{
	//	SDL_PumpEvents(); // NOTE: as per the documentation here: https://wiki.libsdl.org/SDL2/SDL_GetKeyboardState I should cal SDL_PumpEvents before SDL_GetKeyboardState. But as per here: https://wiki.libsdl.org/SDL2/SDL_PumpEvents SDL_PollEvent() above already calls this implicitly. I do it here anyway just to be safe.
	//	return SDL_GetKeyboardState(NULL);
	return nullptr;
}

int GG::InputManager::getMouseX()
{
	return mouseX;
}

int GG::InputManager::getMouseY()
{
	return mouseY;
}

void GG::InputManager::notifyKeyEventListeners(GG::KeyEventType keyEventType, SDL_Keycode key)
{
	for (GG::KeyEventListener* keyEventListenerPtr : keyEventListeners)
	{
		keyEventListenerPtr->onKeyEvent(keyEventType, key);
	}
}

void GG::InputManager::notifyMouseEventListeners(GG::MouseEventType mouseEventType)
{
	pollMousePosition();

	for (GG::MouseEventListener* mouseEventListenerPtr : mouseEventListeners)
	{
		mouseEventListenerPtr->onMouseEvent(mouseEventType, mouseX, mouseY);
	}
}

void GG::InputManager::notifyTextInputEventListeners(std::string text)
{
	for (GG::TextInputEventListener* textInputEventListenerPtr : textInputEventListeners)
	{
		textInputEventListenerPtr->onTextInputEvent(text);
	}
}

void GG::InputManager::notifyScrollEventListeners(int mouseX, int mouseY, float scrollX, float scrollY)
{
	for (GG::ScrollEventListener* scrollEventListenerPtr : scrollEventListeners)
	{
		scrollEventListenerPtr->onScrollEvent(mouseX, mouseY, scrollX, scrollY);
	}
}

void GG::InputManager::pollMousePosition()
{
	float windowMouseX, windowMouseY;
	SDL_GetMouseState(&windowMouseX, &windowMouseY);

	float logicalMouseX, logicalMouseY;
	SDL_RenderCoordinatesFromWindow(renderer, windowMouseX, windowMouseY, &logicalMouseX, &logicalMouseY);

	mouseX = (int)logicalMouseX;
	mouseY = (int)logicalMouseY;
}
