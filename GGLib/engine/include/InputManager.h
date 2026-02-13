#pragma once

#include <SDL.h>

#include <list>
#include <memory>
#include <string>

enum KeyEventType
{
	KEY_DOWN,
	KEY_DOWN_REPEAT,
	KEY_UP
};

enum MouseEventType
{
	MOUSE_MOVE,

	LEFT_MOUSE_DOWN,
	LEFT_MOUSE_UP,

	RIGHT_MOUSE_DOWN,
	RIGHT_MOUSE_UP
};

class MouseEventListener
{
public:
	virtual void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) = 0;
};

class KeyEventListener
{
public:
	virtual void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) = 0;
};

class TextInputEventListener
{
public:
	virtual void onTextInputEvent(const std::string text) = 0;
};

class InputManager
{
private:
	std::list<MouseEventListener*> mouseEventListeners;
	std::list<KeyEventListener*> keyEventListeners;
	std::list<TextInputEventListener*> textInputEventListeners;

	// most recently polled mouse position (in logical coords)
	int mouseX, mouseY;

	SDL_Renderer* renderer = nullptr;

public:
	void init(SDL_Renderer* renderer);

	bool pollEvents();

	void addKeyEventListener(KeyEventListener* keyEventListener);
	void removeKeyEventListener(KeyEventListener* keyEventListener);

	void addMouseEventListener(MouseEventListener* mouseEventListener);
	void removeMouseEventListener(MouseEventListener* mouseEventListener);

	void addTextInputEventListener(TextInputEventListener* textInputEventListener);
	void removeTextInputEventListener(TextInputEventListener* textInputEventListener);

	const Uint8* getKeyboardState();
	int getMouseX();
	int getMouseY();

private:
	void notifyKeyEventListeners(KeyEventType keyEventType, SDL_Keycode key); // TODO abstract away usage of "SDL"_Keycode --> make your own (just alias it using typedef)? It shouldn't be SDL's.
	void notifyMouseEventListeners(MouseEventType mouseEventType);
	void notifyTextInputEventListeners(const std::string text);

	void pollMousePosition();
};
