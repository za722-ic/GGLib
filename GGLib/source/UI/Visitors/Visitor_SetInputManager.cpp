#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

// TODO: not all controls need to listen to all events.
// TODO: we need to remove controls when they are destroyed --> need another vistor for that
void Visitor_SetInputManager::visitForControl(Control* control)
{
	inputManager->addKeyEventListener(control);
	inputManager->addMouseEventListener(control);
	inputManager->addTextInputEventListener(control);
	inputManager->addScrollEventListener(control);
}

void Visitor_SetInputManager::visitForLabel(Label* label)
{
	visitForControl(label);
}

void Visitor_SetInputManager::visitForFlexContainer(Container* container)
{
	return;
}

void Visitor_SetInputManager::visitForAbsoluteContainer(Container* container)
{
	return;
}
