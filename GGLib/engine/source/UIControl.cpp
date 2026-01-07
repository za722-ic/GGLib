#include "UIControl.h"

InputManager *UIControl::inputManager = nullptr;
std::vector<UIControl*> UIControl::uiControls;

UIControl::UIControl()
{
	inputManager->addMouseEventListener(this);  // TODO: at present, *every* control becomes a mouselistener, even if they don't actually do anything with mouse events (unlikely, but possible) --> any easy way to optimise this? (only bother if there is lag)
	uiControls.push_back(this);
}

UIControl::~UIControl()
{
	inputManager->removeMouseEventListener(this);
}
