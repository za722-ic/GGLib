#pragma once

#include "InputManager.h"
#include "Button.h"

class UIManager
{
private:
	InputManager* inputManager = nullptr;


public:
	void init(InputManager* _inputManager)
	{
		inputManager = _inputManager;

		Button::inputManager = _inputManager;
	}
};
