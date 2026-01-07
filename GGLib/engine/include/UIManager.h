#pragma once

#include "InputManager.h"
#include "UIControl.h"

class UIManager
{
private:
	InputManager* inputManager = nullptr;


public:
	void init(InputManager* _inputManager)
	{
		inputManager = _inputManager;

		UIControl::inputManager = _inputManager;
	}

	void renderUI(Canvas* canvas)
	{
		for (UIControl* uiControl : UIControl::uiControls)
			uiControl->render(canvas);
	}
};
